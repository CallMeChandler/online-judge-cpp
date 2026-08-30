#include <crow.h>

#include "service/ProblemService.hpp"

crow::json::wvalue serializeProblem(const Problem& problem) {
    crow::json::wvalue json;

    json["id"] = problem.id;
    json["title"] = problem.title;
    json["description"] = problem.description;
    json["difficulty"] = problem.difficulty;

    for (size_t i = 0; i < problem.tags.size(); i++){
        json["tags"][i] = problem.tags[i];
    }

    for (size_t i = 0; i < problem.constraints.size(); i++){
        json["constraints"][i] = problem.constraints[i];
    }

    for (size_t i = 0; i < problem.sample_test_cases.size(); i++){
        json["sample_test_cases"][i]["input"] = problem.sample_test_cases[i].input;

        json["sample_test_cases"][i]["expected_output"] = problem.sample_test_cases[i].expected_output;
    }

    return json;
}

int main() {
    crow::SimpleApp app;

    ProblemRepository repository("../data/problems.json");
    ProblemService service(repository);

    CROW_ROUTE(app, "/health")([] {
        crow::json::wvalue response;
        response["status"] = "ok";
        return response;
    });

    CROW_ROUTE(app, "/problems")([&service]{
        auto problems = service.getAllProblems();

        crow::json::wvalue response;

        for (size_t i = 0; i < problems.size(); i++){
            response[i] = serializeProblem(problems[i]);
        }

        return response;
    });

    CROW_ROUTE(app, "/problems/<int>")(
        [&service](int id) {
            auto problem  = service.getProblemById(id);

            if (!problem){
                return crow::response(404, "Problem not found");
            }

            return crow::response(serializeProblem(*problem));
        }
    );

    app.port(8080).multithreaded().run();
}