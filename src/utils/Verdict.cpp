#include "utils/Verdict.hpp"

#include <stdexcept>

std::string verdictToString(Verdict verdict) {
    switch (verdict) {
        case Verdict::NONE:
            return "NONE";

        case Verdict::AC:
            return "AC";

        case Verdict::WA:
            return "WA";

        case Verdict::RE:
            return "RE";

        case Verdict::CE:
            return "CE";

        case Verdict::TLE:
            return "TLE";
    }

    throw std::invalid_argument("Invalid verdict");
}

Verdict verdictFromString(const std::string& verdict) {
    if (verdict == "NONE") {
        return Verdict::NONE;
    }

    if (verdict == "AC") {
        return Verdict::AC;
    }

    if (verdict == "WA") {
        return Verdict::WA;
    }

    if (verdict == "RE") {
        return Verdict::RE;
    }

    if (verdict == "CE") {
        return Verdict::CE;
    }

    if (verdict == "TLE") {
        return Verdict::TLE;
    }

    throw std::invalid_argument(
        "Invalid verdict: " + verdict
    );
}
