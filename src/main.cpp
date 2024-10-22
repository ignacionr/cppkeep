#include <keep_client.hpp>
#include <iostream>

int main() {
    // Create a new Keep client

    // obtain the client_id from environment
    char* client_id_env = nullptr;
    size_t len = 0;
    if (_dupenv_s(&client_id_env, &len, "ORGANIZER_CLIENT_ID") || client_id_env == nullptr) {
        throw std::runtime_error("Error: ORGANIZER_CLIENT_ID environment variable not set.");
    }
    std::string client_id{client_id_env, len};

    // obtain the client_secret from environment
    char* client_secret_env = nullptr;
    if (_dupenv_s(&client_secret_env, &len, "ORGANIZER_CLIENT_SECRET") || client_secret_env == nullptr) {
        throw std::runtime_error("Error: ORGANIZER_CLIENT_SECRET environment variable not set.");
    }
    std::string client_secret{client_secret_env, len};

    KeepClient client(client_id, client_secret);

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