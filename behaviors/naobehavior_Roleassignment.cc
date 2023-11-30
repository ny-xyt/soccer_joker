//pt
#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"
#include<algorithm>
#include <sstream>
#include<bits/stdc++.h>

#include "mmdr.h"
#include "mmdr.cc"


void NaoBehavior::beam(double &beamX, double &beamY, double &beamAngle) {
   switch(worldModel->getUNum()) {
        case 1:
            beamX = -14.5;
            beamY = 0;
            beamAngle = 0;
            break;
        //defender
        case 2 :
            beamX = -10;
            beamY = 3;
            beamAngle = 0;
            break;
        case 3 :
            beamX = -10;
            beamY = 0;
            beamAngle = 0;
            break;
        case 4:
            beamX = -10;
            beamY = -3;
            beamAngle = 0;
            break;
        //midfielder
        case 5:
            beamX = -6;
            beamY = 7;
            beamAngle = 0;
            break;
        case 6:
            beamX = -5.5;
            beamY = 0;
            beamAngle = 0;
            break;
        case 7:
        beamX = -6;
            beamY = -7;
            beamAngle = 0;
            
            break;
            //forward
        case 8:
            beamX = -0.3;
            beamY = 7;
            beamAngle = 0;
            break;
        case 9:
            beamX = -2;
            beamY = 0;
            beamAngle = 0;
            break;
        case 10:
            beamX = -0.3;
            beamY = -8;
            beamAngle = 0;
            break;
        case 11:
            beamX = -0.4;
            beamY = -3;
            beamAngle = 0;
    }
}


map<int, Role> NaoBehavior::getAgentRoles() {
    map<int, Role> agentRoles;
    set <Role> assignedRoles;

    agentRoles[chooseCF()] = HELPER;
    assignedRoles.insert(HELPER);
    if (!agentRoles.count(1)) {
        // Assign goalie the goalie role if goalie is not already assigned a role
        agentRoles[1] = GOALIE;
        assignedRoles.insert(GOALIE);
    }
    // Simple greedy role assignment of remaining unassigned roles
    typedef std::list <std::pair<double, std::pair < int, Role>>> dist_to_roles_list;
    dist_to_roles_list agentsDistancesToRoles;
    for (int r = 0; r < NUM_ROLES; r++) {
        Role role = static_cast<Role>(r);
        if (assignedRoles.count(role)) {
            continue;
        }
        for (int i = 1; i <= NUM_AGENTS; i++) {
            if (agentRoles.count(i)) {
                continue;
            }
            agentsDistancesToRoles.push_back(make_pair(getAgentDistanceToRole(i, role), make_pair(i, role)));
        }
    }

    agentsDistancesToRoles.sort();

    for (dist_to_roles_list::iterator it = agentsDistancesToRoles.begin(); it != agentsDistancesToRoles.end(); ++it) {
        pair<int, Role> assignment = it->second;
        int agent = assignment.first;
        Role role = assignment.second;
        if (agentRoles.count(agent) || assignedRoles.count(role)) {
            continue;
        }
        agentRoles[agent] = role;
        assignedRoles.insert(role);
        if (agentRoles.size() == NUM_AGENTS) {
            break;
        }
    }

    return agentRoles;
}
//每个球员根据其角色提供适当的目标位置
VecPosition NaoBehavior::getSpaceForRole(Role role) {
    
    worldModel->getRVSender()->clear();
    worldModel->getRVSender()->clearStaticDrawings();
    
    VecPosition ball = worldModel->getBall();
    if (beamablePlayMode()) {
        ball = VecPosition(0, 0, 0);
    }
    ball.setZ(0);

    // Keep ball position on the field
    ball.setX(max(min(ball.getX(), HALF_FIELD_X), -HALF_FIELD_X));
    ball.setY(max(min(ball.getY(), HALF_FIELD_Y), -HALF_FIELD_Y));
    VecPosition posball = worldModel->getBall();
    VecPosition space = VecPosition(0,0,0);
    int closestopp = getOpponentClosestToBall();
    VecPosition opp_pos = worldModel->getOpponent(WO_OPPONENT1 + closestopp - 1);
    double closestoppdist = ball.getDistanceTo(opp_pos);
    if(closestoppdist < ball.getDistanceTo(worldModel->getTeammate(chooseCF())) && ball.getX() > -2 && !getinpassmode) defendmode = true;
    else defendmode = false;
    switch(role) {
    // case ON_BALL:
    //     space = ball;
    //     break;
    // case GOALIE:
    //     space = VecPosition(-HALF_FIELD_X + 0.5, 0, 0);
    //     break;
    // case SUPPORTER: //前腰
    //     space = VecPosition(ball.getX()>-8?ball.getX()-1.5:ball.getX()+1.5, ball.getY()+(ball.getY()>0?1:-1), 0);
    //     break;
    // case BACK_LEFT://边后卫
    //     space = VecPosition(-8,ball.getY()>5?2:ball.getY()+1,0);
    //     break;
    // case BACK_RIGHT:
    //     space = VecPosition(-8,ball.getY()<-5?-2:ball.getY()-1,0);
    //     break;
    // //mid position
    // case MID_LEFT://左中锋
    //     space = VecPosition(ball.getX()>-5?ball.getX()-2:0,4,0);
    //     break;
    // case MID_RIGHT://右中锋
    //     space = VecPosition(ball.getX()>-5?ball.getX()-2:0,-4,0);
    //     break;
    // case WING_LEFT:
    //     space = VecPosition(ball.getX()>-7?ball.getX():2,7,0);
    //     break;
    // case WING_RIGHT:
    //     space = VecPosition(ball.getX()>-7?ball.getX():2,-7,0);
    //     break;
    // case FORWARD_LEFT://左前锋
    //     space = VecPosition(ball.getX()>-8?ball.getX()-2:ball.getX()+2,1.5,0);
    //     break;
    // case FORWARD_RIGHT://右前锋
    //     space = VecPosition(ball.getX()>-8?ball.getX()-2:ball.getX()+2,-1.5,0);
    //     break;
    // default:
    //     cerr << "Unknown role: " << role << "\n";
    case HELPER: //补球哥
        if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
            space = VecPosition(-0.2, -3, 0);
        else
            space = VecPosition(12, 0, 0);
        break;
    case GOALIE: //守门员
        space = VecPosition(-HALF_FIELD_X + 0.5, 0, 0);
        break;
    case FORWARD_MID: //中前锋
        if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
            space = VecPosition(-2,0,0);
        else
            space = VecPosition(6,0,0);
        break;
    case FORWARD_LEFT://左前锋
        if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
            space = VecPosition(-0.2,7,0);
        else
            space = VecPosition(6,6,0);
        break;
    case FORWARD_RIGHT://右前锋
        if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
            space = VecPosition(-0.2,-8,0);
        else
            space = VecPosition(6,-6,0);
        break;
    case BACK_MID: //中后卫
            space = VecPosition(-10, 0, 0);
        break;
    case BACK_LEFT://左后卫
        space = VecPosition(-10,3,0);
        break;
    case BACK_RIGHT: //右后卫
        space = VecPosition(-10,-3,0);
        break;
    case MID_LEFT://左中场
        if(posball.getX()<0.2)
            space = VecPosition(-5,6,0);
        else
            space = VecPosition(0,5,0);
        break;
    case MID_RIGHT://右中场
        if(posball.getX()<0.2)
            space = VecPosition(-6,-7,0);
        else
            space = VecPosition(0,-5,0);
    
        break;
    case MID_MID://中场
        if(posball.getX()<0.2)
            space = VecPosition(-5.5,0,0);
        else
        space = VecPosition(0,0,0);
        break;
    default:
        cerr << "Unknown role: " << role << "\n";
    }

    space.setX(max(-HALF_FIELD_X, min(HALF_FIELD_X, space.getX())));
    space.setY(max(-HALF_FIELD_Y, min(HALF_FIELD_Y, space.getY())));

    if (beamablePlayMode()) {
        // Stay within your own half on kickoffs
        space.setX(min(-0.5, space.getX()));
    }

    return space;
}

double NaoBehavior::getAgentDistanceToRole(int uNum, Role role) {
    VecPosition temp;
    if (worldModel->getUNum() == uNum) {
        // This is us
        temp = worldModel->getMyPosition();
    } else {
        WorldObject *teammate = worldModel->getWorldObject(WO_TEAMMATE1 + uNum - 1);
        temp = teammate->pos;
    }
    temp.setZ(0);

    double distanceToRole = temp.getDistanceTo(getSpaceForRole(role));
    return distanceToRole;
}


int NaoBehavior::chooseCF()
{

    double angle = 0;
    // WorldObject ball;
    // if (worldModel->getWorldObject(WO_BALL)->absVel.getMagnitude() < 0.12) {
    //     ball.pos = worldModel->getWorldObject(WO_BALL)->pos;
    // } else {
    //     ball.pos = VecPosition(
    //             worldModel->getWorldObject(WO_BALL)->pos.getX() + worldModel->getWorldObject(WO_BALL)->absVel.getX(),
    //             worldModel->getWorldObject(WO_BALL)->pos.getY() + worldModel->getWorldObject(WO_BALL)->absVel.getY());
    // }

    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;
    for(int i = WO_TEAMMATE2; i < WO_TEAMMATE1+NUM_AGENTS; ++i) {
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum) {
            // This is us
            temp = worldModel->getMyPosition();
            angle = worldModel->getMyAngDeg();
        } else {
            WorldObject* teammate = worldModel->getWorldObject( i );
            if (teammate->validPosition) {
                angle = worldModel->getWorldObject(i)->orien;
                temp = teammate->pos;
            } else {
                continue;
            }
        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(VecPosition(11, 0, 0));
        
        if(worldModel->getFallenTeammate(i - WO_TEAMMATE1)){
            distanceToBall += 100;
        }
        if (distanceToBall < closestDistanceToBall) {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }  

    if(playerClosestToBall == -1) return 2;
    return playerClosestToBall;
}


map<int, Role> NaoBehavior::RoleAssign() {
    const int playerNum = 10;
    Role Robot[10] = {BACK_MID, BACK_LEFT, BACK_RIGHT, MID_LEFT, MID_RIGHT, MID_MID, FORWARD_MID, FORWARD_LEFT,
                      FORWARD_RIGHT, HELPER};
    map<int, Role> agentRoles;//Set players to their roles
    Test t;

    for (int i = 0; i < playerNum; i++) {
        VecPosition space = getSpaceForRole(Robot[i]);
        Point Roleposition = std::make_pair<double, double>(space.getX(), space.getY());
        t.targets.push_back(Roleposition);
    }

    for (int i = WO_TEAMMATE2; i <= WO_TEAMMATE11; i++) {
        VecPosition temp;
        if (worldModel->getUNum() == i) {
            Point Agentposition = std::make_pair<double, double>(worldModel->getMyPosition().getX(),
                                                                 worldModel->getMyPosition().getY());
            t.starts.push_back(Agentposition);
        } else {
            WorldObject *teammate = worldModel->getWorldObject(i);
            if (teammate->validPosition) {
                Point Agentposition = std::make_pair<double, double>(teammate->pos.getX(), teammate->pos.getY());
                t.starts.push_back(Agentposition);
            } else {
                Point Agentposition = std::make_pair<double, double>(0, 0);
                t.starts.push_back(Agentposition);
            }
        }
    }
    std::vector <Edge> answerMap = SOLVER(t);
    for (int i = 0; i < 10; i++) {
        agentRoles[answerMap[i].second.first + 2] = Robot[answerMap[i].second.second];
    }
    agentRoles[1] = GOALIE;

    return agentRoles;
}
