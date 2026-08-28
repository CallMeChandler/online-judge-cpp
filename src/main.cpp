#include <crow.h>

int main(){
    crow::SimpleApp app;

    CROW_ROUTE(app, "/health")
    ([] {
        crow::json::wvalue response;

        response["status"] = "ok";

        return response;
    });

    app.port(8080).multithreaded().run();

    return 0;

}