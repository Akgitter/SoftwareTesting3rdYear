
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600 // Vista+ to ensure InetNtopA is declared
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib") // ignored by MinGW, used by MSVC

using namespace std;

struct UrlParts {
    string scheme;
    string host;
    string port; // optional
    string path;
};

static inline string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

UrlParts parseUrl(const string &input) {
    string s = input;
    UrlParts u;
    u.scheme = "";
    u.host = "";
    u.port = "";
    u.path = "/";

    string lower = s;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    size_t pos = string::npos;
    if ((pos = lower.find("://")) != string::npos) {
        u.scheme = lower.substr(0, pos);
        s = s.substr(pos + 3);
    }

    size_t slash = s.find('/');
    string hostport = (slash == string::npos) ? s : s.substr(0, slash);
    if (slash != string::npos) u.path = s.substr(slash);

    size_t at = hostport.rfind('@');
    if (at != string::npos) hostport = hostport.substr(at + 1);

    if (!hostport.empty() && hostport.front() == '[') {
        size_t close = hostport.find(']');
        if (close != string::npos) {
            u.host = hostport.substr(0, close + 1);
            if (close + 1 < hostport.size() && hostport[close + 1] == ':') {
                u.port = hostport.substr(close + 2);
            }
        } else {
            u.host = hostport;
        }
    } else {
        size_t colon = hostport.find(':');
        if (colon != string::npos) {
            u.host = hostport.substr(0, colon);
            u.port = hostport.substr(colon + 1);
        } else {
            u.host = hostport;
        }
    }

    if (u.port.empty()) {
        if (u.scheme == "https") u.port = "443";
        else if (u.scheme == "http") u.port = "80";
    }

    if (u.host.empty()) u.host = input;
    return u;
}

// Resolve host to printable addresses using InetNtopA for Windows compatibility
vector<string> resolveHost(const string &host, const string &port) {
    vector<string> results;
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo *res = nullptr;
    int rc = getaddrinfo(host.c_str(), port.empty() ? nullptr : port.c_str(), &hints, &res);
    if (rc != 0) {
        return results;
    }

    char buf[INET6_ADDRSTRLEN] = {0};
    for (addrinfo *p = res; p != nullptr; p = p->ai_next) {
        if (p->ai_family == AF_INET) {
            sockaddr_in *ipv4 = (sockaddr_in *)p->ai_addr;
            if (InetNtopA(AF_INET, &(ipv4->sin_addr), buf, sizeof(buf))) {
                results.push_back(string(buf));
            }
        } else if (p->ai_family == AF_INET6) {
            sockaddr_in6 *ipv6 = (sockaddr_in6 *)p->ai_addr;
            if (InetNtopA(AF_INET6, &(ipv6->sin6_addr), buf, sizeof(buf))) {
                results.push_back(string(buf));
            }
        }
    }
    freeaddrinfo(res);
    return results;
}

bool checkTcpConnect(const string &host, const string &port, int timeoutSeconds = 5) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo *res = nullptr;
    int rc = getaddrinfo(host.c_str(), port.c_str(), &hints, &res);
    if (rc != 0) return false;

    bool ok = false;
    for (addrinfo *p = res; p != nullptr; p = p->ai_next) {
        SOCKET sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock == INVALID_SOCKET) continue;

        int tv = timeoutSeconds * 1000;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
        setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tv, sizeof(tv));

        if (connect(sock, p->ai_addr, (int)p->ai_addrlen) == 0) {
            ok = true;
            closesocket(sock);
            break;
        }
        closesocket(sock);
    }
    freeaddrinfo(res);
    return ok;
}

string fetchHttpHeaders(const string &host, const string &port, const string &path) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo *res = nullptr;
    int rc = getaddrinfo(host.c_str(), port.c_str(), &hints, &res);
    if (rc != 0 || res == nullptr) return string();

    string headers;
    for (addrinfo *p = res; p != nullptr; p = p->ai_next) {
        SOCKET sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock == INVALID_SOCKET) continue;

        if (connect(sock, p->ai_addr, (int)p->ai_addrlen) != 0) {
            closesocket(sock);
            continue;
        }

        ostringstream req;
        req << "GET " << (path.empty() ? "/" : path) << " HTTP/1.0\r\n";
        req << "Host: " << host << "\r\n";
        req << "User-Agent: securityTesting/1.0\r\n";
        req << "Accept: */*\r\n";
        req << "Connection: close\r\n\r\n";
        string reqStr = req.str();

        int sent = 0;
        while (sent < (int)reqStr.size()) {
            int s = send(sock, reqStr.c_str() + sent, (int)reqStr.size() - sent, 0);
            if (s == SOCKET_ERROR) break;
            sent += s;
        }
        if (sent < (int)reqStr.size()) {
            closesocket(sock);
            continue;
        }

        const int BUF_SIZE = 4096;
        vector<char> buf(BUF_SIZE);
        string recvBuf;
        int r;
        while ((r = recv(sock, buf.data(), BUF_SIZE, 0)) > 0) {
            recvBuf.append(buf.data(), r);
            if (recvBuf.find("\r\n\r\n") != string::npos) break;
            if (recvBuf.size() > 65536) break;
        }
        closesocket(sock);

        if (recvBuf.empty()) continue;

        size_t hpos = recvBuf.find("\r\n\r\n");
        if (hpos != string::npos) headers = recvBuf.substr(0, hpos + 2);
        else headers = recvBuf;

        break;
    }
    freeaddrinfo(res);
    return headers;
}

map<string, string> parseHeaders(const string &headerBlock) {
    map<string, string> m;
    istringstream ss(headerBlock);
    string line;
    if (!getline(ss, line)) return m;
    if (!line.empty() && (line.back() == '\r')) line.pop_back();
    m["Status"] = trim(line);
    while (getline(ss, line)) {
        if (!line.empty() && (line.back() == '\r')) line.pop_back();
        if (line.empty()) break;
        size_t colon = line.find(':');
        if (colon == string::npos) continue;
        string name = line.substr(0, colon);
        string value = line.substr(colon + 1);
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        m[name] = trim(value);
    }
    return m;
}

vector<pair<string, bool>> checkCommonSecurityHeaders(const map<string, string> &hdrs) {
    vector<string> keys = {
        "strict-transport-security",
        "content-security-policy",
        "x-frame-options",
        "x-content-type-options",
        "referrer-policy"
    };
    vector<pair<string, bool>> res;
    for (auto &k : keys) {
        res.push_back({k, hdrs.find(k) != hdrs.end()});
    }
    return res;
}

int main() {
    cout << "Website Security Quick Tester\n";
    cout << "--------------------------------\n";
    cout << "Enter website URL or host (e.g. example.com or http://example.com/path): ";
    string input;
    if (!getline(cin, input) || trim(input).empty()) {
        cerr << "No input provided. Exiting.\n";
        return 1;
    }
    input = trim(input);

    WSADATA wsaData;
    int wsaErr = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (wsaErr != 0) {
        cerr << "WSAStartup failed: " << wsaErr << "\n";
        return 1;
    }

    UrlParts u = parseUrl(input);
    cout << "Parsed URL parts:\n";
    cout << "  Scheme: " << (u.scheme.empty() ? "(none)" : u.scheme) << "\n";
    cout << "  Host  : " << u.host << "\n";
    cout << "  Port  : " << (u.port.empty() ? "(none)" : u.port) << "\n";
    cout << "  Path  : " << u.path << "\n";

    string hostForResolve = u.host;
    if (!hostForResolve.empty() && hostForResolve.front() == '[' && hostForResolve.back() == ']') {
        hostForResolve = hostForResolve.substr(1, hostForResolve.size() - 2);
    }

    cout << "Resolving host...\n";
    vector<string> ips = resolveHost(hostForResolve, u.port);
    if (ips.empty()) cout << "  Could not resolve host.\n";
    else {
        for (size_t i = 0; i < ips.size(); ++i) cout << "  " << i+1 << ": " << ips[i] << "\n";
    }

    cout << "Checking TCP reachability...\n";
    bool port80 = checkTcpConnect(hostForResolve, "80");
    bool port443 = checkTcpConnect(hostForResolve, "443");
    cout << "  Port 80 (HTTP)  : " << (port80 ? "open/reachable" : "closed/unreachable") << "\n";
    cout << "  Port 443 (HTTPS): " << (port443 ? "open/reachable" : "closed/unreachable") << "\n";

    if (port80) {
        cout << "Fetching HTTP headers (via port 80) for path: " << u.path << "\n";
        string hdrBlock = fetchHttpHeaders(hostForResolve, "80", u.path);
        if (hdrBlock.empty()) {
            cout << "  Failed to fetch HTTP headers or server did not respond with headers.\n";
        } else {
            auto hdrs = parseHeaders(hdrBlock);
            cout << "\nReceived status: " << (hdrs.count("Status") ? hdrs["Status"] : "(unknown)") << "\n";

            cout << "Response headers:\n";
            for (auto &p : hdrs) {
                if (p.first == "Status") continue;
                cout << "  " << p.first << ": " << p.second << "\n";
            }

            auto checks = checkCommonSecurityHeaders(hdrs);
            cout << "\nCommon security headers presence:\n";
            for (auto &c : checks) {
                cout << "  " << c.first << ": " << (c.second ? "present" : "MISSING") << "\n";
            }

            if (hdrs.find("x-content-type-options") == hdrs.end()) {
                cout << "\nNote: Without X-Content-Type-Options: nosniff, some browsers may perform MIME sniffing.\n";
            }
            if (hdrs.find("strict-transport-security") == hdrs.end() && port443) {
                cout << "Note: Site serves HTTP but has no HSTS header; consider redirecting HTTP->HTTPS and enabling HSTS on HTTPS.\n";
            }
        }
    } else {
        cout << "Skipping HTTP header checks because port 80 is not reachable.\n";
    }

    cout << "\nQuick check complete. For full security testing use dedicated tools and obtain permission before scanning any site you don't own.\n";
    WSACleanup();
    return 0;
}