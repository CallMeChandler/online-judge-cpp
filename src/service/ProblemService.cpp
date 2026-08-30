#include "service/ProblemService.hpp"

ProblemService::ProblemService(ProblemRepository repository)
    : repository(std::move(repository)) {}

std::vector<Problem> ProblemService::getAllProblems() const {
    return repository.getAll();
}

std::optional<Problem> ProblemService::getProblemById(int id) const {
    return repository.getById(id);
}