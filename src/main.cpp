#include <crow.h>
#include <iostream>
#include <fstream>

#include "service/ProblemService.hpp"
#include "service/SubmissionService.hpp"
#include "utils/SubmissionStatus.hpp"
#include "queue/BlockingQueue.hpp"
#include "worker/WorkerPool.hpp"
#include "models/JudgeJob.hpp"
#include "repository/ExecutionRepository.hpp"
#include "judge/Judge.hpp"

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

    SubmissionRepository submission_repository(
        "../data/submissions.json"
    );

    ExecutionRepository execution_repository(
        "../data/executions.json"
    );

    BlockingQueue<JudgeJob> judge_queue;

    SubmissionService submission_service(
        submission_repository,
        service,
        judge_queue
    );

    Judge judge(
        submission_repository,
        execution_repository,
        repository
    );

    WorkerPool worker_pool(
        judge_queue,
        3,
        [&judge](const JudgeJob& job) {
            judge.judge(job);
        }
    );

    CROW_ROUTE(app, "/")([] {
        std::ifstream file("../public/index.html");

        if (!file.is_open()) {
            return crow::response(500, "Failed to load index.html");
        }

        std::string html((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

        crow::response res(html);
        res.set_header("Content-Type", "text/html");
        return res;
    });

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

    CROW_ROUTE(app, "/submissions")
    .methods(crow::HTTPMethod::POST)
    ([&submission_service](const crow::request& request) {

        try {
            auto body = crow::json::load(request.body);

            if (!body) {
                return crow::response(400, "Invalid JSON");
            }

            if (!body.has("problem_id") ||
                !body.has("source_code") ||
                !body.has("language")) {
                return crow::response(
                    400,
                    "Missing required fields"
                );
            }

            int problem_id = body["problem_id"].i();

            std::string source_code =
                body["source_code"].s();

            std::string language =
                body["language"].s();

            Submission submission =
                submission_service.createSubmission(
                    problem_id,
                    source_code,
                    language
                );

            crow::json::wvalue response;

            response["submission_id"] = submission.id;
            response["status"] = "QUEUED";

            return crow::response(202, response);

        } catch (const std::invalid_argument& error) {

            return crow::response(
                400,
                error.what()
            );

        } catch (const std::exception& error) {

            return crow::response(
                500,
                error.what()
            );
        }
    });

    CROW_ROUTE(app, "/submissions/<string>")
    ([&submission_service](const std::string& id) {

        auto submission =
            submission_service.getSubmissionById(id);

        if (!submission) {
            return crow::response(
                404,
                "Submission not found"
            );
        }

        crow::json::wvalue response;

        response["submission_id"] = submission->id;
        response["problem_id"] = submission->problem_id;
        response["language"] = submission->language;
        response["status"] = submissionStatusToString(submission->status);

        return crow::response(response);
    });

    worker_pool.start();

    app.port(8080).multithreaded().run();

    worker_pool.shutdown();

    return 0;
}

