#include <scoreCounter.h>

#include <algorithm>

wok::ScoreCounter::ScoreCounter(int lastHighscore) :
    lastHighscore(lastHighscore), currentScore(0)
{ }

wok::ScoreCounter::ScoreCounter() :
    lastHighscore(0), currentScore(0)
{ }

void wok::ScoreCounter::addPoints(int points)
{
    if (points <= 0) return;

    currentScore += points;
}

auto wok::ScoreCounter::getPoints() -> int
{
    return currentScore;
}

auto wok::ScoreCounter::getHighscore() -> int
{
    return std::max(currentScore, lastHighscore);
}

auto wok::ScoreCounter::wasHighscoreBeat() -> int
{
    return currentScore > lastHighscore;
}
