#include <keep_client.hpp>
#include <iostream>

int main() {
    // Create a new Keep client
    KeepClient client("YOUR_CLIENT_ID", "YOUR_CLIENT_SECRET");

    // Authenticate using OAuth 2.0
    if (!client.authenticate("YOUR_USERNAME", "YOUR_PASSWORD")) {
        std::cerr << "Authentication failed" << std::endl;
        return 1;
    }

    // Create a new note
    if (!client.createNote("Hello, World!", "This is a test note")) {
        std::cerr << "Note creation failed" << std::endl;
        return 1;
    }

    // Get all notes
    std::string notes = client.getNotes();
    std::cout << notes << std::endl;

    return 0;
}