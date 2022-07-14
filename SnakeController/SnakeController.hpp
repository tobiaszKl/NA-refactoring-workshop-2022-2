#pragma once

#include <list>
#include <memory>
#include <stdexcept>

#include "IEventHandler.hpp"
#include "SnakeInterface.hpp"

class Event;
class IPort;

namespace Snake
{
struct ConfigurationError : std::logic_error
{
    ConfigurationError();
};

struct UnexpectedEventException : std::runtime_error
{
    UnexpectedEventException();
};

class Controller : public IEventHandler
{
public:
    Controller(IPort& p_displayPort, IPort& p_foodPort, IPort& p_scorePort, std::string const& p_config);

    Controller(Controller const& p_rhs) = delete;
    Controller& operator=(Controller const& p_rhs) = delete;

    void receive(std::unique_ptr<Event> e) override;

private:
    void handleTimePassed(const TimeoutInd&);
    void handleDirectionChange(const DirectionInd&);
    void handleFoodPositionChange(const FoodInd& receivedFood);
    void handleNewFood(const FoodResp& requestedFood);

    struct Segment
    {
        int x;
        int y;
        int ttl;
    };

    Segment getNewHead() const;
    bool doesCollideWithSnake(const Segment& newSegment) const;
    bool doesCollideWithWall(const Segment& newSegment) const;
    bool doesCollideWithFood(const Segment& newSegment) const;

    void notifyAboutFailure();
    void repaintTile(const Segment& position, Cell type);
    void repaintTile(unsigned int x, unsigned int y, Cell type);

    void cleanNotExistingSnakeSegments();


    IPort& m_displayPort;
    IPort& m_foodPort;
    IPort& m_scorePort;

    std::pair<int, int> m_mapDimension;
    std::pair<int, int> m_foodPosition;

    Direction m_currentDirection;
    std::list<Segment> m_segments;

    enum EventId {
        timeoutId = 0x20,
        directionId = 0x10,
        foodIndId = 0x40,
        foodRespId = 0x42
    };
};

} // namespace Snake
