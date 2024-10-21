#ifndef KEEP_CLIENT_H
#define KEEP_CLIENT_H

#include <string>
#include <curl/curl.h>

class KeepClient {
public:
    KeepClient(const std::string& clientId, const std::string& clientSecret);
    ~KeepClient();

    // Authenticate using OAuth 2.0
    bool authenticate(const std::string& username, const std::string& password);

    // Create a new note
    bool createNote(const std::string& title, const std::string& content);

    // Get all notes
    std::string getNotes();

private:
    std::string clientId_;
    std::string clientSecret_;
    std::string accessToken_;
    CURL* curl_;
};

#endif  // KEEP_CLIENT_H
