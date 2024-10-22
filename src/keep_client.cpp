#include "keep_client.hpp"
#include <iostream>
#include <string>
#include <curl/curl.h>

// Callback function to write response data into a std::string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

KeepClient::KeepClient(const std::string& clientId, const std::string& clientSecret)
    : clientId_(clientId), clientSecret_(clientSecret), curl_(curl_easy_init()) {}

KeepClient::~KeepClient() {
    curl_easy_cleanup(curl_);
}

bool KeepClient::authenticate(const std::string& username, const std::string& password) {
    // Set up the authentication request
    std::string authUrl = "https://accounts.google.com/o/oauth2/v2/auth";
    std::string authBody = "client_id=" + clientId_ + "&response_type=code&redirect_uri=urn:ietf:wg:oauth:2.0:oob&scope=https://www.googleapis.com/auth/keep";
    std::string authHeader = "Content-Type: application/x-www-form-urlencoded";

    // Send the authentication request
    std::string authCode;
    curl_easy_setopt(curl_, CURLOPT_URL, authUrl.c_str());
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, authBody.c_str());
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &authCode);
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, authHeader.c_str());
    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);

    // Get the authorization code
    CURLcode res = curl_easy_perform(curl_);
    if (res != CURLE_OK) {
        std::cerr << "Authentication failed: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    // Exchange the authorization code for an access token
    std::string tokenUrl = "https://oauth2.googleapis.com/token";
    std::string tokenBody = "code=" + std::string(authCode) + "&client_id=" + clientId_ + "&client_secret=" + clientSecret_ + "&redirect_uri=urn:ietf:wg:oauth:2.0:oob&grant_type=authorization_code";
    std::string tokenHeader = "Content-Type: application/x-www-form-urlencoded";

    // Send the token request
    curl_easy_setopt(curl_, CURLOPT_URL, tokenUrl.c_str());
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, tokenBody.c_str());
    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, tokenHeader.c_str());

    // Get the access token
    std::string accessToken;
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &accessToken);
    res = curl_easy_perform(curl_);
    if (res != CURLE_OK) {
        std::cerr << "Token exchange failed: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    accessToken_ = std::string(accessToken);
    return true;
}

bool KeepClient::createNote(const std::string& title, const std::string& content) {
    // Set up the note creation request
    std::string noteUrl = "https://www.googleapis.com/keep/v1/notes";
    std::string noteBody = "{\"title\":\"" + title + "\",\"content\":\"" + content + "\"}";
    std::string noteHeader = "Content-Type: application/json; charset=UTF-8";

    // Add the access token to the header
    noteHeader += "; Authorization: Bearer " + accessToken_;

    // Send the note creation request
    curl_easy_setopt(curl_, CURLOPT_URL, noteUrl.c_str());
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, noteBody.c_str());
    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, noteHeader.c_str());

    CURLcode res = curl_easy_perform(curl_);
    if (res != CURLE_OK) {
        std::cerr << "Note creation failed: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    return true;
}

std::string KeepClient::getNotes() {
    // Set up the note retrieval request
    std::string noteUrl = "https://www.googleapis.com/keep/v1/notes";
    std::string noteHeader = "Authorization: Bearer " + accessToken_;

    // Send the note retrieval request
    curl_easy_setopt(curl_, CURLOPT_URL, noteUrl.c_str());
    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, noteHeader.c_str());

    std::string notes;
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &notes);
    CURLcode res = curl_easy_perform(curl_);
    if (res != CURLE_OK) {
        std::cerr << "Note retrieval failed: " << curl_easy_strerror(res) << std::endl;
        return "";
    }

    return notes;
}