#include <physics\collisions.h>

struct AABBEdges
{
    float left, right, top, bottom;
    AABBEdges(sf::FloatRect box)
    {
        left = box.left;
        right = box.left + box.width;
        top = box.top;
        bottom = box.top + box.height;
    }
};

bool checkIfWeCollideWithVerticalEdge(AABBEdges penetrator, float edge)
{
    if (penetrator.left > edge) return false; // We are off to the right
    if (penetrator.right < edge) return false; // We are off to the left

    return true;
}

bool checkIfWeCollideWithHorizontalEdge(AABBEdges penetrator, float edge)
{
    if (penetrator.top > edge) return false; // We are bellow
    if (penetrator.bottom < edge) return false; // We are above

    return true;
}

auto wok::collide::AABBWithAABB(sf::FloatRect penetratorRect, sf::FloatRect receiverRect) -> Reaction
{
    sf::FloatRect intersection;
    if (!penetratorRect.intersects(receiverRect, intersection))
        // Two rects do not intersect
        return {};

    sf::Vector2f penetratorCentre(
        penetratorRect.left + penetratorRect.width / 2.f,
        penetratorRect.top + penetratorRect.height / 2.f
    );

    sf::Vector2f receiverCentre(
        receiverRect.left + receiverRect.width / 2.f,
        receiverRect.top + receiverRect.height / 2.f
    );

    sf::Vector2f targetsOffset = penetratorCentre - receiverCentre;

    AABBEdges penetrator(penetratorRect);
    AABBEdges receiver(receiverRect);

    // Case: One object is fully inside another
    if (intersection == penetratorRect || intersection == receiverRect)
    {
        // We check on what side of the object the penetratorRect is on
        if (targetsOffset.x < targetsOffset.y == -targetsOffset.x < targetsOffset.y)
        {
            // Top/Bottom
            if (targetsOffset.y < 0)
            {
                // Above
                return { { 0.f, penetrator.bottom - receiver.top } };
            }
            else
            {
                // Below
                return { { 0.f, penetrator.top - receiver.bottom } };
            }
        }
        else
        {
            // Left/Right
            if (targetsOffset.x < 0)
            {
                // Left
                return { { penetrator.right - receiver.left, 0.f } };
            }
            else
            {
                // Right
                return { { penetrator.left - receiver.right, 0.f } };
            }
        }
    }

    // Case: Penetrator intersects with an edge of the receiver
    sf::Vector2f reaction;
    if (checkIfWeCollideWithVerticalEdge(penetrator, receiver.left))
    {
        reaction.x = penetrator.right - receiver.left;
    }
    else if (checkIfWeCollideWithVerticalEdge(penetrator, receiver.right))
    {
        reaction.x = penetrator.left - receiver.right;
    }

    if (checkIfWeCollideWithHorizontalEdge(penetrator, receiver.top))
    {
        reaction.y = penetrator.bottom - receiver.top;
    }
    else if (checkIfWeCollideWithHorizontalEdge(penetrator, receiver.bottom))
    {
        reaction.y = penetrator.top - receiver.bottom;
    }

    if (reaction != sf::Vector2f())
        return { reaction };
    else
        return { };
}
