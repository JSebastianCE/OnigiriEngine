#pragma once

#include "ECS/Actor.h"
#include <vector>

/**
 * @class A_Racer
 * @brief NPC corredor que sigue una ruta con comportamientos de steering.
 */
class A_Racer : public Actor {
public:

  A_Racer(const std::string& name, int initPlace);

  void 
  update(float deltaTime) override;

  void 
  setWaypoints(const std::vector<sf::Vector2f>& waypoints);

  void
  resetPosition();

  void 
  setSpeed(float newSpeed);

  int   
  getCurrentWaypointIndex() const { return currentWaypointIndex; }

  float 
  getDistanceToNextWaypoint() const;



  int  
  getLap() const { return lapCount; }

  bool  
  isFinished() const { return finished; }

  void  
  markFinished(bool v) { finished = v; }


private:
  int initialPositionIndex = 0;
  int currentWaypointIndex = 0;

  int  lapCount = 0;
  int  prevWaypointIndex = 0;
  bool finished = false;

  float speed = 100.0f;
  float maxSpeed = 200.0f;
  float steeringForce = 10.0f;

  std::vector<sf::Vector2f> m_waypoints;

  void 
  seekToCurrentWaypoint(float deltaTime);
};
