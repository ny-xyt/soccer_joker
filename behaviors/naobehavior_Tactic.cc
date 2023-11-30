//pt
#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"
#include<algorithm>
#include <sstream>
#include<bits/stdc++.h>

extern int agentBodyType;
 who_close who_close_t_1;
int NaoBehavior::getRoleNum(Role role) {
    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i) {
        if (agentRoles[i] == role) return i;
        else return 1;
    }

}

SkillType NaoBehavior::judegment_playmode() {
    double playerClosestToBall = 1;
    double closestDistanceToBall = 99999999;
    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i) {
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum) {
            temp = worldModel->getMyPosition();
        } else {
            WorldObject *teammate = worldModel->getWorldObject(i);
            if (teammate->validPosition) {
                temp = teammate->pos;
            } else {
                continue;
            }
        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);
        if (distanceToBall < closestDistanceToBall) {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }  //计算距离球最近的队友球员的球员号码（playerClosestToBall）和到球的最短距离（closestDistanceToBall）
    }

    if ((worldModel->getPlayMode() == PM_KICK_OFF_LEFT && worldModel->getSide() == SIDE_LEFT)
        || (worldModel->getPlayMode() == PM_KICK_OFF_RIGHT && worldModel->getSide() == SIDE_RIGHT)) {
        if (worldModel->getUNum() == 9)
            return kickBall(KICK_8M, VecPosition(-5, -7, 0)); //发球
        else { return SKILL_STAND; }
    } else if ((worldModel->getPlayMode() == PM_FREE_KICK_LEFT && worldModel->getSide() == SIDE_LEFT)
               || (worldModel->getPlayMode() == PM_FREE_KICK_RIGHT && worldModel->getSide() == SIDE_RIGHT)) 
               {
        if (worldModel->getUNum() == playerClosestToBall)
            return kickBall(SHOOT_UT ,  VecPosition(15,1,0)); //任意球
        else return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()])); 
    } else if ((worldModel->getPlayMode() == PM_KICK_IN_LEFT && worldModel->getSide() == SIDE_LEFT)
               || (worldModel->getPlayMode() == PM_KICK_IN_RIGHT && worldModel->getSide() == SIDE_RIGHT)) 
               {
        if (worldModel->getUNum() == playerClosestToBall)
        {
              if(ball.getX()<=4)
                {
                    return pass();
                }
                else if(ball.getX()>=13&&ball.getX()<=15)
                {
                    return kickBall(KICK_8M,getRolePosition(MID_LEFT).getX()+0.5);
                }
                else if(ball.getY()>-10&&ball.getY()<10)
                {
                      return kickBall(KICK_8M,getRolePosition(MID_RIGHT).getX()+0.5);
                }
                else
                {
                    return shoot();
                }
            
        }
        if (worldModel->getUNum() != playerClosestToBall)
            return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
    }
    else if ((worldModel->getPlayMode() == PM_CORNER_KICK_LEFT && worldModel->getSide() == SIDE_LEFT)
             || (worldModel->getPlayMode() == PM_CORNER_KICK_RIGHT && worldModel->getSide() == SIDE_RIGHT)) {
        if (worldModel->getUNum() == playerClosestToBall)
            return kickBall(TONGDA_KICK_12M, VecPosition(15, -3, 0));
        if (worldModel->getUNum() != playerClosestToBall)
            return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
    }
    else if ((worldModel->getPlayMode() == PM_GOAL_KICK_LEFT && worldModel->getSide() == SIDE_LEFT)
             || (worldModel->getPlayMode() == PM_GOAL_KICK_RIGHT && worldModel->getSide() == SIDE_RIGHT)) {
        if (agentRoles[worldModel->getUNum()] == GOALIE) return kickBall(KICK_FORWARD, VecPosition(15, 0, 0));
        else return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
    } else if 
    ((worldModel->getPlayMode() == PM_PASS_LEFT && worldModel->getSide() == SIDE_LEFT))
     {
        if (worldModel->getUNum() == playerClosestToBall)
        {
            return pass();
        }
        if (worldModel->getUNum() != playerClosestToBall)
            return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));

    } 
    // else if ((worldModel->getPlayMode() == PM_PASS_RIGHT && worldModel->getSide() == SIDE_RIGHT))
    //  {
    //     if (worldModel->getUNum() == playerClosestToBall) {
    //         return pass();
    //     }
    //     if (worldModel->getUNum() != playerClosestToBall)
    //         return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
    // }
    else {
        for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i) {
            int playerNum = i - WO_TEAMMATE1 + 1;
            if (worldModel->getUNum() == playerNum) {
                if (getSpaceForRole(agentRoles[worldModel->getUNum()]).getDistanceTo(ball) < 2)
                    return goToTargetRelative(VecPosition(),
                                              atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
                else return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
            }
        }
    }


}


SkillType NaoBehavior::SpecialMode() {
    if (worldModel->getUNum() == 4) return goToTarget(VecPosition(-12, 2, 0));
    else if (worldModel->getUNum() == 9) return goToTarget(VecPosition(-12, -3, 0));
    else if (worldModel->getUNum() == 3) return goToTarget(VecPosition(-12, -2, 0));
    else return SKILL_STAND;
}

SkillType NaoBehavior::goToSpace(VecPosition space) {
    const double SPACE_THRESH = 0.5;

    // Adjust target to not be too close to teammates or the ball
    VecPosition target = collisionAvoidance(true /*teammate*/, true/*opponent*/, false/*ball*/, 1/*proximity thresh*/,
                                            .5/*collision thresh*/, space, true/*keepDistance*/);
    return goToTarget(target);
}


int NaoBehavior::getPlayerClosestToBall() {
    VecPosition posball = worldModel->getBall();
    // Find closest player to ball
    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;
    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i) {
        VecPosition temp;
        VecPosition mypos;
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum) {
            // This is us
            temp = worldModel->getMyPosition();
            mypos = worldModel->getMyPosition();
        } else {
            WorldObject *teammate = worldModel->getWorldObject(i);
            if (teammate->validPosition) {
                temp = teammate->pos;
            } else {
                continue;
            }
        }
        temp.setZ(0);
        double distanceToBall = temp.getDistanceTo(ball);
        // if(worldModel->getFallenTeammate(playerNum))
        // {
        //     distanceToBall=distanceToBall+100;
            
        // }
        // if(posball.getX()<mypos.getX()-0.5)
        // {
        //     distanceToBall=distanceToBall+100;
            
        // }
        if (distanceToBall < closestDistanceToBall) {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }
    return playerClosestToBall;
}

int NaoBehavior::getPlayerClosestToBall_1() {
    // Find closest player to ball
    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;
    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i) {
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum) {
            // This is us
            temp = worldModel->getMyPosition();
        } else {
            WorldObject *teammate = worldModel->getWorldObject(i);
            if (teammate->validPosition) {
                temp = teammate->pos;
            } else {
                continue;
            }
        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);
        if (distanceToBall < closestDistanceToBall) {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }
    return playerClosestToBall;
}

int NaoBehavior::getOpponentClosestToBall() {
    // Find closest player to ball
    int opponentClosestToBall = -1;
    double closestDistanceToBall = 10000;
    for (int i = WO_OPPONENT1; i < WO_OPPONENT1 + NUM_AGENTS; ++i) {
        VecPosition temp;
        int opponentNum = i - WO_OPPONENT1 + 1;
        WorldObject *opponent = worldModel->getWorldObject(i);
        if (opponent->validPosition) {
            temp = opponent->pos;
        } else {
            continue;
        }
        temp.setZ(0);
        double distanceToBall = temp.getDistanceTo(ball);
        if (me.getX() < -1) {
            if (distanceToBall < closestDistanceToBall) {
                opponentClosestToBall = opponentNum;
                closestDistanceToBall = distanceToBall;
            }
        } else {
            if (distanceToBall < closestDistanceToBall && !worldModel->getFallenOpponent(i)) {
                opponentClosestToBall = opponentNum;
                closestDistanceToBall = distanceToBall;
            }
        }

    }
    return opponentClosestToBall;
}

int NaoBehavior::getOpponentClosestToRole(VecPosition space) {
    // Find closest player to ball
    int opponentClosestToSpace = -1;
    double closestDistanceToSpace = 10000;
    for (int i = WO_OPPONENT1; i < WO_OPPONENT1 + NUM_AGENTS; ++i) {
        VecPosition temp;
        int opponentNum = i - WO_OPPONENT1 + 1;
        WorldObject *opponent = worldModel->getWorldObject(i);
        if (opponent->validPosition) {
            temp = opponent->pos;
        } else {
            continue;
        }
        temp.setZ(0);
        double distanceToSpace = temp.getDistanceTo(space);
        if (me.getX() < -1) {
            if (distanceToSpace < closestDistanceToSpace) {
                opponentClosestToSpace = opponentNum;
                closestDistanceToSpace = distanceToSpace;
            }
        } else {
            if (distanceToSpace < closestDistanceToSpace && !worldModel->getFallenOpponent(i)) {
                opponentClosestToSpace = opponentNum;
                closestDistanceToSpace = distanceToSpace;
            }
        }

    }
    return opponentClosestToSpace;
}






//right->0;left->1, forward->2
SkillType NaoBehavior::shoot() {

    int closestopp = getOpponentClosestToBall();
    VecPosition opp_pos = worldModel->getOpponent(WO_OPPONENT1 + closestopp - 1);
    if (me.getDistanceTo(VecPosition(15, 0, 0)) < 3) {
        return kickBall(SHOOT_UT, VecPosition(15, 0, 0));
    } else if (me.getDistanceTo(VecPosition(15, 0, 0)) < 5) {
        return kickBall(SHOOT_UT, VecPosition(15, 0, 0));
    } else if (me.getDistanceTo(VecPosition(15, 0, 0)) < 12) {
        return kickBall(TONGDA_KICK_12M, VecPosition(15, 0, 0));
    } else {
        return kickBall(SHOOT_UT, VecPosition(15, 0, 0));
    }
}






//   首先，检查球的位置 ball.getX()，如果球的 X 坐标小于 -4，表示球位于中线附近，它会将 getinpassmode 标志设置为 true，表示进入传球模式，然后选择 pass() 技能，即传球。
//   否则，如果球的 X 坐标大于等于 -4，它会选择 kickBall(KICK_DRIBBLE, VecPosition(15, 0, 0)) 技能，即带球盘带并朝向 (15, 0, 0) 的方向行进。

// {

// if (me.getX() > 0){
//     return shoot();
// } 
// else{
//     if(ball.getX() < -4 ){
//         getinpassmode = true;
//         return pass();
//      }
//     else
//         return kickBall(KICK_DRIBBLE, VecPosition(15, 0, 0));
// }



// }





VecPosition NaoBehavior::GK_Tactic_pass() {
    VecPosition pos = VecPosition(15, 0, 0);
    return pos;
}






SkillType NaoBehavior::HELPER_Tactic()
{
    who_close_to_ball();
    int  opp_less_place=calculateZoneDensities_A();
    VecPosition posball = worldModel->getBall();
    int closestopp = getOpponentClosestToBall();
    VecPosition opp_pos = worldModel->getOpponent(WO_OPPONENT1 + closestopp - 1);
    if(posball.getX()>10&&posball.getX()<15&&posball.getY()>-5&&posball.getY()<5)
    {
        // if(posball.getX()>14)
        // {
            if(getPlayerClosestToBall()==worldModel->getUNum())
            {
                return kickBall(KICK_DRIBBLE, VecPosition(15, 0, 0));
            }
            else return cy_findball(HELPER);
        // }
        // else if(posball.getX()>13)
        // {
        //     if(getPlayerClosestToBall()==worldModel->getUNum())
        //     {
        //         return kickBall(KICK_IK, VecPosition(15, 0, 0));
        //     }
        //     else return cy_findball(HELPER);
        // }
        // else
        // {
        //     if(getPlayerClosestToBall()==worldModel->getUNum())
        //     {
        //         return kickBall(SHOOT_UT, VecPosition(15, 0, 0));
        //     }
        //     else return cy_findball(HELPER);
        // }
        
    }
    if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5) {
        // close enough to turn to the ball
        return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
    } else {
        // not close to the target
        return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
    }
   
//     else if(who_close_t_1.our_closest_player!=worldModel->getUNum())
//     {
//      if(posball.getX()<0&&posball.getX()>-15&&posball.getY()<10&&posball.getY()>-10)
//      {
//        return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
//      }
//      else if(posball.getX()>0&&posball.getX()<9)
//      {
//          if(opp_less_place==1||opp_less_place==2)       
//     {
//                   return goToSpace(VecPosition(11,3,0));
//     }        
//     else if(opp_less_place==3||opp_less_place==4)   
//     {
//                  return goToSpace(VecPosition(11,-3,0));
//     }
//      }
//      else if(posball.getX()>=9&&posball.getX()<15&&posball.getY()>7&&posball.getY()<10)
//     {  
//         if (me.getDistanceTo( VecPosition(12.5,-4)) < 0.5)
//         {
//           return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
//     } 
//     else
//      {
//         // not close to the target
//         relse if(who_close_t_1.our_closest_player!=worldModel->getUNum())
//     {
//      if(posball.getX()<0&&posball.getX()>-15&&posball.getY()<10&&posball.getY()>-10)
//      {
//        return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
//      }
//      else if(posball.getX()>0&&posball.getX()<9)
//      {
//          if(opp_less_place==1||opp_less_place==2)       
//     {
//                   return goToSpace(VecPosition(11,3,0));
//     }        
//     else if(opp_less_place==3||opp_less_place==4)   
//     {
//                  return goToSpace(VecPosition(11,-3,0));
//     }
//      }
//      else if(posball.getX()>=9&&posball.getX()<15&&posball.getY()>7&&posball.getY()<10)
//     {  
//         if (me.getDistanceTo( VecPosition(12.5,-4)) < 0.5)
//         {
//           return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
//     } 
//     else
//      {
//         // not close to the target
//         return goToSpace( VecPosition(12.5,-4));
//       }
//     }
// else if(posball.getX()>=9&&posball.getX()<15&&posball.getY()<-7&&posball.getY()>-10)
//     {  
//         if (me.getDistanceTo( VecPosition(12.5,4)) < 0.5)
//         {
//           return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
//         }
//      else 
//        {
//         // not close to the target
//         return goToSpace( VecPosition(12.5,4));
//     }
//     }
//     else
//     {
//          return kickBall(KICK_DRIBBLE, VecPosition(15, 0.5, 0));
//     }
//   }
// else
// {
//        if (me.getX()>=13&&me.getX()<15&&me.getY()>-5&&me.getY()<5)
//     {
//          if(me.getY()>=2.5&&me.getY()<5&&me.getX()>=14&&me.getX()<15)
//          {
//             return kickBall(KICK_IK,VecPosition(14.5,0,0));
//          }
//         if(me.getY()<-2.5&&me.getY()>-5&&me.getX()>=14&&me.getX()<15)
//          {
//             return kickBall(KICK_IK,VecPosition(14.5,0,0));
//          }
//         else if(me.getY()>0)
//         {    getinpassmode=false;
//             return kickBall(KICK_DRIBBLE, VecPosition(15, 0.5, 0));
//         }
//         else if (me.getY()<=0)
//         {    getinpassmode=false;
//             return kickBall(KICK_DRIBBLE, VecPosition(15, 0.5, 0));
//         }
//     }
//      else if (me.getX() > 13 && me.getX() < 15 && me.getY() <= -5 && me.getY() > -10)
//      {
//         if(me.getX()<=ball.getX())
//         {
//         // return kickBall(TONGDA_KICK_12M, getRolePosition(WING_LEFT).getX()+0.5);
//        if(getRolePosition(MID_LEFT).getX()>me.getX())
//        {
//         if(distanceBetweenPoints(me,getRolePosition(MID_LEFT).getX())<=9)
//         {
//              return kickBall(SHOOT_UT,getRolePosition(MID_LEFT).getX()+0.5);
//         }else
//         {
//               return kickBall(SHOOT_UT,getRolePosition(MID_LEFT).getX()+0.5);
//         }
//        }
//        else
//        {
//         return  kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//        }
//         }
//         else
//         {
//             return  kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//         }
// }
//      else if(me.getX()>=11&&me.getX()<13&&me.getY()<7&&me.getY()>-7)
//      {
//             if(me.getY()>=2.5&&me.getY()<7)
//             {    getinpassmode=false;
//                 return shoot();
//             }
//             else if(me.getY()<2.5&&me.getY()>-2.5)
//             {getinpassmode=false;
//                 return kickBall(KICK_IK,VecPosition(15,0, 0));
//             }
//             else if(me.getY()<=-2.5&&me.getY()>-7)
//             {    getinpassmode=false;
//                 return kickBall(SHOOT_UT,VecPosition(15,0, 0));
//             }
//         }
//     else
//     {
//          return shoot();
//     }
   
// }eturn goToSpace( VecPosition(12.5,-4));
//       }
//     }
// else if(posball.getX()>=9&&posball.getX()<15&&posball.getY()<-7&&posball.getY()>-10)
//     {  
//         if (me.getDistanceTo( VecPosition(12.5,4)) < 0.5)
//         {
//           return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
//         }else if(who_close_t_1.our_closest_player!=worldModel->getUNum())
//     {
//      if(posball.getX()<0&&posball.getX()>-15&&posball.getY()<10&&posball.getY()>-10)
//      {
//        return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
//      }
//      else if(posball.getX()>0&&posball.getX()<9)
//      {
//          if(opp_less_place==1||opp_less_place==2)       
//     {
//                   return goToSpace(VecPosition(11,3,0));
//     }        
//     else if(opp_less_place==3||opp_less_place==4)   
//     {
//                  return goToSpace(VecPosition(11,-3,0));
//     }
//      }
//      else if(posball.getX()>=9&&posball.getX()<15&&posball.getY()>7&&posball.getY()<10)
//     {  
//         if (me.getDistanceTo( VecPosition(12.5,-4)) < 0.5)
//         {
//           return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
//     } 
//     else
//      {
//         // not close to the target
//         return goToSpace( VecPosition(12.5,-4));
//       }
//     }
// else if(posball.getX()>=9&&posball.getX()<15&&posball.getY()<-7&&posball.getY()>-10)
//     {  
//         if (me.getDistanceTo( VecPosition(12.5,4)) < 0.5)
//         {
//           return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
//         }
//      else 
//        {
//         // not close to the target
//         return goToSpace( VecPosition(12.5,4));
//     }
//     }
//     else
//     {
//          return kickBall(KICK_DRIBBLE, VecPosition(15, 0.5, 0));
//     }
//   }
// else
// {
//        if (me.getX()>=13&&me.getX()<15&&me.getY()>-5&&me.getY()<5)
//     {
//          if(me.getY()>=2.5&&me.getY()<5&&me.getX()>=14&&me.getX()<15)
//          {
//             return kickBall(KICK_IK,VecPosition(14.5,0,0));
//          }
//         if(me.getY()<-2.5&&me.getY()>-5&&me.getX()>=14&&me.getX()<15)
//          {
//             return kickBall(KICK_IK,VecPosition(14.5,0,0));
//          }
//         else if(me.getY()>0)
//         {    getinpassmode=false;
//             return kickBall(KICK_DRIBBLE, VecPosition(15, 0.5, 0));
//         }
//         else if (me.getY()<=0)
//         {    getinpassmode=false;
//             return kickBall(KICK_DRIBBLE, VecPosition(15, 0.5, 0));
//         }
//     }
//      else if (me.getX() > 13 && me.getX() < 15 && me.getY() <= -5 && me.getY() > -10)
//      {
//         if(me.getX()<=ball.getX())
//         {
//         // return kickBall(TONGDA_KICK_12M, getRolePosition(WING_LEFT).getX()+0.5);
//        if(getRolePosition(MID_LEFT).getX()>me.getX())
//        {
//         if(distanceBetweenPoints(me,getRolePosition(MID_LEFT).getX())<=9)
//         {
//              return kickBall(SHOOT_UT,getRolePosition(MID_LEFT).getX()+0.5);
//         }else
//         {
//               return kickBall(SHOOT_UT,getRolePosition(MID_LEFT).getX()+0.5);
//         }
//        }
//        else
//        {
//         return  kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//        }
//         }
//         else
//         {
//             return  kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//         }
// }
//      else if(me.getX()>=11&&me.getX()<13&&me.getY()<7&&me.getY()>-7)
//      {
//             if(me.getY()>=2.5&&me.getY()<7)
//             {    getinpassmode=false;
//                 return shoot();
//             }
//             else if(me.getY()<2.5&&me.getY()>-2.5)
//             {getinpassmode=false;
//                 return kickBall(KICK_IK,VecPosition(15,0, 0));
//             }
//             else if(me.getY()<=-2.5&&me.getY()>-7)
//             {    getinpassmode=false;
//                 return kickBall(SHOOT_UT,VecPosition(15,0, 0));
//             }
//         }
//     else
//     {
//          return shoot();
//     }
   
// }
//      else 
//        {
//         // not close to the target
//         return goToSpace( VecPosition(12.5,4));else if(who_close_t_1.our_closest_player!=worldModel->getUNum())
//     {
//      if(posball.getX()<0&&posball.getX()>-15&&posball.getY()<10&&posball.getY()>-10)
//      {
//        return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
//      }
//      else if(posball.getX()>0&&posball.getX()<9)
//      {
//          if(opp_less_place==1||opp_less_place==2)       
//     {
//                   return goToSpace(VecPosition(11,3,0));
//     }        
//     else if(opp_less_place==3||opp_less_place==4)   
//     {
//                  return goToSpace(VecPosition(11,-3,0));
//     }
//      }
//      else if(posball.getX()>=9&&posball.getX()<15&&posball.getY()>7&&posball.getY()<10)
//     {  
//         if (me.getDistanceTo( VecPosition(12.5,-4)) < 0.5)
//         {
//           return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
//     } 
//     else
//      {
//         // not close to the target
//         return goToSpace( VecPosition(12.5,-4));
//       }
//     }
// else if(posball.getX()>=9&&posball.getX()<15&&posball.getY()<-7&&posball.getY()>-10)
//     {  
//         if (me.getDistanceTo( VecPosition(12.5,4)) < 0.5)
//         {
//           return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
//         }
//      else 
//        {
//         // not close to the target
//         return goToSpace( VecPosition(12.5,4));
//     }
//     }
//     else
//     {
//          return kickBall(KICK_DRIBBLE, VecPosition(15, 0.5, 0));
//     }
//   }
// else
// {
//        if (me.getX()>=13&&me.getX()<15&&me.getY()>-5&&me.getY()<5)
//     {
//          if(me.getY()>=2.5&&me.getY()<5&&me.getX()>=14&&me.getX()<15)
//          {
//             return kickBall(KICK_IK,VecPosition(14.5,0,0));
//          }
//         if(me.getY()<-2.5&&me.getY()>-5&&me.getX()>=14&&me.getX()<15)
//          {
//             return kickBall(KICK_IK,VecPosition(14.5,0,0));
//          }
//         else if(me.getY()>0)
//         {    getinpassmode=false;
//             return kickBall(KICK_DRIBBLE, VecPosition(15, 0.5, 0));
//         }
//         else if (me.getY()<=0)
//         {    getinpassmode=false;
//             return kickBall(KICK_DRIBBLE, VecPosition(15, 0.5, 0));
//         }
//     }
//      else if (me.getX() > 13 && me.getX() < 15 && me.getY() <= -5 && me.getY() > -10)
//      {
//         if(me.getX()<=ball.getX())
//         {
//         // return kickBall(TONGDA_KICK_12M, getRolePosition(WING_LEFT).getX()+0.5);
//        if(getRolePosition(MID_LEFT).getX()>me.getX())
//        {
//         if(distanceBetweenPoints(me,getRolePosition(MID_LEFT).getX())<=9)
//         {
//              return kickBall(SHOOT_UT,getRolePosition(MID_LEFT).getX()+0.5);
//         }else
//         {
//               return kickBall(SHOOT_UT,getRolePosition(MID_LEFT).getX()+0.5);
//         }
//        }
//        else
//        {
//         return  kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//        }
//         }
//         else
//         {
//             return  kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//         }
// }
//      else if(me.getX()>=11&&me.getX()<13&&me.getY()<7&&me.getY()>-7)
//      {
//             if(me.getY()>=2.5&&me.getY()<7)
//             {    getinpassmode=false;
//                 return shoot();
//             }
//             else if(me.getY()<2.5&&me.getY()>-2.5)
//             {getinpassmode=false;
//                 return kickBall(KICK_IK,VecPosition(15,0, 0));
//             }
//             else if(me.getY()<=-2.5&&me.getY()>-7)
//             {    getinpassmode=false;
//                 return kickBall(SHOOT_UT,VecPosition(15,0, 0));
//             }
//         }
//     else
//     {
//          return shoot();
//     }
   
// }
//     }
//     }
//     else
//     {
//          return kickBall(KICK_DRIBBLE, VecPosition(15, 0.5, 0));
//     }
//   }
// else
// {
//        if (me.getX()>=13&&me.getX()<15&&me.getY()>-5&&me.getY()<5)
//     {
//          if(me.getY()>=2.5&&me.getY()<5&&me.getX()>=14&&me.getX()<15)
//          {
//             return kickBall(KICK_IK,VecPosition(14.5,0,0));
//          }
//         if(me.getY()<-2.5&&me.getY()>-5&&me.getX()>=14&&me.getX()<15)
//          {
//             return kickBall(KICK_IK,VecPosition(14.5,0,0));
//          }
//         else if(me.getY()>0)
//         {    getinpassmode=false;
//             return kickBall(KICK_DRIBBLE, VecPosition(15, 0.5, 0));
//         }
//         else if (me.getY()<=0)
//         {    getinpassmode=false;
//             return kickBall(KICK_DRIBBLE, VecPosition(15, 0.5, 0));
//         }
//     }
//      else if (me.getX() > 13 && me.getX() < 15 && me.getY() <= -5 && me.getY() > -10)
//      {
//         if(me.getX()<=ball.getX())
//         {
//         // return kickBall(TONGDA_KICK_12M, getRolePosition(WING_LEFT).getX()+0.5);
//        if(getRolePosition(MID_LEFT).getX()>me.getX())
//        {
//         if(distanceBetweenPoints(me,getRolePosition(MID_LEFT).getX())<=9)
//         {
//              return kickBall(SHOOT_UT,getRolePosition(MID_LEFT).getX()+0.5);
//         }else
//         {
//               return kickBall(SHOOT_UT,getRolePosition(MID_LEFT).getX()+0.5);
//         }
//        }
//        else
//        {
//         return  kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//        }
//         }
//         else
//         {
//             return  kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//         }
// }
//      else if(me.getX()>=11&&me.getX()<13&&me.getY()<7&&me.getY()>-7)
//      {
//             if(me.getY()>=2.5&&me.getY()<7)
//             {    getinpassmode=false;
//                 return shoot();
//             }
//             else if(me.getY()<2.5&&me.getY()>-2.5)
//             {getinpassmode=false;
//                 return kickBall(KICK_IK,VecPosition(15,0, 0));
//             }
//             else if(me.getY()<=-2.5&&me.getY()>-7)
//             {    getinpassmode=false;
//                 return kickBall(SHOOT_UT,VecPosition(15,0, 0));
//             }
//         }
//     else
//     {
//          return shoot();
//     }
   
// }
}


SkillType NaoBehavior::GOALIE_Tactic() {
     VecPosition posball = worldModel->getBall();
     // 获取机器人相对于世界坐标系的朝向角度
double robotAngle = worldModel->getMyAngDeg();
// 获取球相对于机器人坐标系的位置
VecPosition ballRelative = worldModel->g2l(ball);
// 获取球相对于机器人的角度
double ballAngle = atan2Deg(ballRelative.getY(), ballRelative.getX());
// 计算机器人与球的角度差


    VecPosition target_ball=VecPosition(posball.getX(), 0, 0);
    target_ball = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target_ball, false /*fKeepDistance*/);
        //defend_time==true;
             if(posball.getX()>-15&&posball.getX()<-12.5&&posball.getY()>0&&posball.getY()<3)
           {
                if(getPlayerClosestToBall()==1)
                {
                    return kickBall(TONGDA_KICK_12M, VecPosition(0, 5, 0));
                }  
                else 
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX(), posball.getY(), 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);
                }
           }
           if(posball.getX()>-15&&posball.getX()<-12.5&&posball.getY()>-3&&posball.getY()<0)
           {
                if(getPlayerClosestToBall()==1)
                {
                    return kickBall(TONGDA_KICK_12M, VecPosition(-7, -7, 0));
                }  
                else 
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX(), posball.getY(), 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);
                }
           }
           
            if((posball.getX()>-15&&posball.getX()<-7&&posball.getY()>-10&&posball.getY()<-2)||(posball.getX()==-15&&posball.getY()<0)) 
            {
                 VecPosition target5 = VecPosition(VecPosition(-14.7, -0.9, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            }
            if((posball.getX()>-15&&posball.getX()<-7&&posball.getY()>2&&posball.getY()<10)||(posball.getX()==-15&&posball.getY()>0)) 
            {
                 VecPosition target5 = VecPosition(VecPosition(-14.7, 0.9, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            }
            else {
                //&& isBallMovingTowardsOurGoal()
     if (isBallMovingQuickly() && isBallMovingTowardsOurGoal()) 
    //  if (isBallMovingQuickly())
    {
        VecPosition goaliePosition = worldModel->getMyPosition();
        double ballY = posball.getY();

        // 判断球是从左侧还是右侧射来
        if(posball.getX()<=-5)
    {
        if (ballY > goaliePosition.getY()) {
            // 球从左侧射来，执行左扑救动作
            //  std::cout << "左扑救"<< std::endl;
             return SKILL_BLOCK_SIDE_LEFT;
        } else if (ballY < goaliePosition.getY()) {
            // 球从右侧射来，执行右扑救动作
            // std::cout << "右扑救"<< std::endl;
            return SKILL_BLOCK_SIDE_RIGHT;
             // return  SKILL_BLOCK_LEFT;
        }
        else{
            // std::cout << "茶腿"<< std::endl;
            return  SKILL_BLOCK_LEFT;
        }
    }
    else
    {
       return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
    }
    }  
       else {
    // 检查机器人是否倒下
    if (worldModel->getFallenTeammate(worldModel->getUNum()) ){
        // 如果机器人倒下了，执行站起来的动作
        return SKILL_STAND;
    } else {
       if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5) {
        // close enough to turn to the ball
        return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
    } else {
        // not close to the target
        return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
    }
    }
}


}
}
SkillType NaoBehavior::BACK_MID_Tactic()
{
    VecPosition posball = worldModel->getBall();

    if(me.getDistanceTo(ball)<3&&posball.getX()<-7)
    {
        // if(getPlayerClosestToBall()==worldModel->getUNum())
        // {
        //     ifpassmode();
        //     return kickBall(TONGDA_KICK_12M, VecPosition(0, 0, 0));
        // }

        // else return cy_findball(BACK_MID);
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            ifpassmode();
           return kickBall(TONGDA_KICK_12M,VecPosition(FORWARD_RIGHT));
        }
        else return cy_findball(BACK_MID);
   
  
        
    }
    if(posball.getX()<-6.5&&posball.getX()>-15&&posball.getY()<10&&posball.getY()>-10)
    {
         if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            ifpassmode();
           return kickBall(TONGDA_KICK_12M,VecPosition(FORWARD_RIGHT));
        }
        else return cy_findball(BACK_MID);
  
        
    }

    if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5) {
        // close enough to turn to the ball
        return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
    } else {
        // not close to the target
        return goToSpace(VecPosition(-12,0,0));
        // return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
    }
}
SkillType NaoBehavior::BACK_LEFT_Tactic() {


  VecPosition posball = worldModel->getBall();
    
    if(me.getDistanceTo(ball)<3&&posball.getX()<-6)
    { 
        // if(getPlayerClosestToBall()==worldModel->getUNum())
        // {
        //     ifpassmode();
        //     return kickBall(TONGDA_KICK_12M, VecPosition(0, 0, 0));
        // }
        // else return cy_findball(BACK_LEFT);
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            ifpassmode();
           return kickBall(TONGDA_KICK_12M,VecPosition(FORWARD_LEFT));
        }
        else return cy_findball(BACK_LEFT);
   
        
    }
    if(posball.getX()<-6&&posball.getX()>-15&&posball.getY()<10&&posball.getY()>0)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            ifpassmode();
           return kickBall(TONGDA_KICK_12M,VecPosition(FORWARD_LEFT));
        }
        else return cy_findball(BACK_LEFT);
   
   
        
    }
   else
   {
    if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5) {
        // close enough to turn to the ball
        return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
    } else {
        // not close to the target
        return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
    }
   }
    //   VecPosition target_ball=VecPosition(posball.getX(), 0, 0);  
    //   target_ball = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target_ball, false /*fKeepDistance*/);
    // if(posball.getX()<-6.5&&posball.getX()>-15&&posball.getY()<10&&posball.getY()>0)
    // { 
    //     cy_findball(BACK_LEFT);
    //     if(posball.getX()>getRolePosition(BACK_LEFT).getX()&&posball.getY()>getRolePosition(BACK_LEFT).getY())
    //     {      
    //                 VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.7, posball.getY()-0.3, 0));
	// 	            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
    //                 return goToTarget(target5);
                    
    //     }
    //     if(posball.getX()<getRolePosition(BACK_LEFT).getX()&&posball.getY()>getRolePosition(BACK_LEFT).getY())
    //     {
                
    //                 VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.7, posball.getY()-0.3, 0));
	// 	            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
    //                 return goToTarget(target5);
                   
           
            
    //     }
    //     if(posball.getX()<getRolePosition(BACK_LEFT).getX()&&posball.getY()<getRolePosition(BACK_LEFT).getY())
    //     {
           
    //                 VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.7, posball.getY()+0.3, 0));
	// 	            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
    //                 return goToTarget(target5);
                   
    //     }
    //     if(posball.getX()>getRolePosition(BACK_LEFT).getX()&&posball.getY()<getRolePosition(BACK_LEFT).getY())
    //     {
           
    //                 VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.7, posball.getY()+0.3, 0));
	// 	            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
    //                 return goToTarget(target5);
            
    //     }
        
    // }

    // if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5) {
    //     // close enough to turn to the ball
    //     return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
    // } else {
    //     // not close to the target
    //     return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
    // }
}
SkillType NaoBehavior::BACK_RIGHT_Tactic() {
    VecPosition posball = worldModel->getBall();
    if(me.getDistanceTo(ball)<3&&posball.getX()<-6)
    {
        // if(getPlayerClosestToBall()==worldModel->getUNum())
        // {
        //     ifpassmode();
        //     return kickBall(TONGDA_KICK_12M, VecPosition(0, 0, 0));
        // }

        // else return cy_findball(BACK_RIGHT);
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            ifpassmode();
           return kickBall(TONGDA_KICK_12M,VecPosition(FORWARD_RIGHT));
        }
        else return cy_findball(BACK_RIGHT);
  
        
    }
    if(posball.getX()<-6&&posball.getX()>-15&&posball.getY()<0&&posball.getY()>-10)
    {
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            ifpassmode();
           return kickBall(TONGDA_KICK_12M,VecPosition(FORWARD_RIGHT));
        }
        else return cy_findball(BACK_RIGHT);
  
        
    }

    if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5) {
        // close enough to turn to the ball
        return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
    } else {
        // not close to the target
        return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
    }
}

SkillType NaoBehavior::MID_LEFT_Tactic()
{
    VecPosition posball = worldModel->getBall();  
    if(posball.getX()<7&&posball.getX()>5&&posball.getY()<10&&posball.getY()>0)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(TONGDA_KICK_12M, VecPosition(15, 0, 0));
        }
        else return cy_findball(MID_LEFT);
    }
    else if(posball.getX()<5&&posball.getX()>-1&&posball.getY()<10&&posball.getY()>0)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            ifpassmode();
             return kickBall(TONGDA_KICK_12M,VecPosition(FORWARD_RIGHT));
        }
        else return cy_findball(MID_LEFT);
    }
    else if(posball.getX()<-1&&posball.getX()>-7&&posball.getY()<10&&posball.getY()>0)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            ifpassmode();
             return kickBall(TONGDA_KICK_12M,VecPosition(FORWARD_LEFT));
        }
        else return cy_findball(MID_LEFT);
    }
    else if(me.getDistanceTo(ball)<3)
    {
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(TONGDA_KICK_12M, VecPosition(15, 0, 0));
        }

        else return cy_findball(MID_LEFT);
  
        
    }
    else if(posball.getX()>9)
    {
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5)
        {
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        } 
        else
        {
             // not close to the target
            return goToSpace(VecPosition(5,5,0));
        }
    }
    else if(posball.getX()<0)
    {
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5) 
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        else 
             // not close to the target
            return goToSpace(VecPosition(-4, 5, 0));
    
   }
   else
    {
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5) 
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        else 
             // not close to the target
            return goToSpace(VecPosition(0, 6, 0));
    
   }


}
SkillType NaoBehavior::MID_RIGHT_Tactic()
{
    VecPosition posball = worldModel->getBall(); 
    if(posball.getX()<7&&posball.getX()>5&&posball.getY()<0&&posball.getY()>-10)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(TONGDA_KICK_12M, VecPosition(15, 0, 0));
        }
        else return cy_findball(MID_RIGHT);
    }
    else if(posball.getX()<5&&posball.getX()>-1&&posball.getY()<0&&posball.getY()>-10)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            ifpassmode();
           return kickBall(TONGDA_KICK_12M,VecPosition(FORWARD_LEFT));
        }
        else return cy_findball(MID_RIGHT);
    }
    else if(posball.getX()<-1&&posball.getX()>-7&&posball.getY()<0&&posball.getY()>-10)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            ifpassmode();
           return kickBall(TONGDA_KICK_12M,VecPosition(FORWARD_RIGHT));
        }
        else return cy_findball(MID_RIGHT);
    }
    else if(me.getDistanceTo(ball)<3)
    {
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            
            return kickBall(TONGDA_KICK_12M, VecPosition(15, 0, 0));
        }

        else return cy_findball(MID_RIGHT);
  
        
    }
    else if(posball.getX()>9)
    {
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5)
        {
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        } 
        else
        {
             // not close to the target
            return goToSpace(VecPosition(5,-5,0));
        }
    }
    else if(posball.getX()<0)
    {
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5) 
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        else 
             // not close to the target
            return goToSpace(VecPosition(-4, -5, 0));
    
   }
   else
    {
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5) 
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        else 
             // not close to the target
            return goToSpace(VecPosition(0, -6, 0));
    
   }
}
SkillType NaoBehavior::MID_MID_Tactic()
{
    VecPosition posball = worldModel->getBall(); 
    if(posball.getX()<7&&posball.getX()>5&&posball.getY()<7&&posball.getY()>-7)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(TONGDA_KICK_12M, VecPosition(15, 0, 0));
        }
        else return cy_findball(MID_MID);
    }
    else if(posball.getX()<5&&posball.getX()>-7&&posball.getY()<7&&posball.getY()>-7)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            ifpassmode();
            return kickBall(TONGDA_KICK_12M, VecPosition(FORWARD_RIGHT));
        }
        else return cy_findball(MID_MID);
    }
    else if(me.getDistanceTo(ball)<3)
    {
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(TONGDA_KICK_12M, VecPosition(15, 0, 0));
        }

        else return cy_findball(MID_MID);
  
        
    }
    else if(posball.getX()>9)
    {
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5)
        {
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        } 
        else
        {
             // not close to the target
            return goToSpace(VecPosition(6,0,0));
        }
    }

    else if(posball.getX()<0&&posball.getX()>-7)
    {
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5) 
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        else 
             // not close to the target
            return goToSpace(VecPosition(-4, 0, 0));
    
   }
   else if(posball.getX()<-7)
    {
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5) 
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        else 
             // not close to the target
            return goToSpace(VecPosition(-7, 0, 0));
    
   }
   else
    {
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5) 
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        else 
             // not close to the target
            return goToSpace(VecPosition(-1, 0, 0));
    
   }
}

SkillType NaoBehavior::FORWARD_RIGHT_Tactic()
{
    VecPosition posball = worldModel->getBall(); 
    if(posball.getX()<15&&posball.getX()>13&&posball.getY()<0&&posball.getY()>-10)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(KICK_DRIBBLE, VecPosition(15, 0, 0));
        }
        else return cy_findball(FORWARD_RIGHT);
    }
    
    else if(posball.getX()<13&&posball.getX()>9&&posball.getY()<0&&posball.getY()>-10)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(SHOOT_UT, VecPosition(15, 0, 0));
        }
        else return cy_findball(FORWARD_RIGHT);
    }
    else if(posball.getX()<9&&posball.getX()>3&&posball.getY()<0&&posball.getY()>-10)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(TONGDA_KICK_12M, VecPosition(15, 0, 0));
        }
        else return cy_findball(FORWARD_RIGHT);
    }
    else if(me.getDistanceTo(ball)<3)
    {
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(TONGDA_KICK_12M, VecPosition(15, 0, 0));
        }

        else return cy_findball(FORWARD_RIGHT);
  
        
    }
    else if(posball.getX()>9&&posball.getY()<10&&posball.getY()>0)
    { 
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5)
        {
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        } 
        else
        {
             // not close to the target
             return goToSpace(VecPosition(10,-3.5,0));
        }
      
    }
    else if(posball.getX()<-7)
    {
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5)
        {
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        } 
        else
        {
             // not close to the target
            return goToSpace(VecPosition(3,-6,0));
        }
    }
    else 
    {
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5)
        {
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        } 
        else
        {
             // not close to the target
            return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
        }
    }
}
SkillType NaoBehavior::FORWARD_MID_Tactic()
{
    VecPosition posball = worldModel->getBall(); 
    if(posball.getX()<15&&posball.getX()>13)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(KICK_DRIBBLE, VecPosition(15, 0, 0));
        }
        else return cy_findball(FORWARD_MID);
    }
    else if(posball.getX()<13&&posball.getX()>9)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(SHOOT_UT, VecPosition(15, 0, 0));
        }
        else return cy_findball(FORWARD_MID);
    }
    else if(posball.getX()<9&&posball.getX()>2)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(TONGDA_KICK_12M, VecPosition(15, 0, 0));
        }
        else return cy_findball(FORWARD_MID);
    }
    else if(me.getDistanceTo(ball)<3)
    {
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(TONGDA_KICK_12M, VecPosition(15, 0, 0));
        }

        else return cy_findball(FORWARD_MID);
  
        
    }
    else
    {
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5)
        {
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        } 
        else
        {
             // not close to the target
            return goToSpace(VecPosition(3,0,0));
        }
    }
}
SkillType NaoBehavior::FORWARD_LEFT_Tactic()
{
    VecPosition posball = worldModel->getBall(); 
    if(posball.getX()<15&&posball.getX()>13&&posball.getY()<10&&posball.getY()>0)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(KICK_DRIBBLE, VecPosition(15, 0, 0));
        }
        else return cy_findball(FORWARD_LEFT);
    }
    else if(posball.getX()<13&&posball.getX()>9&&posball.getY()<10&&posball.getY()>0)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(SHOOT_UT, VecPosition(15, 0, 0));
        }
        else return cy_findball(FORWARD_LEFT);
    }
    else if(posball.getX()<9&&posball.getX()>3&&posball.getY()<10&&posball.getY()>0)
    { 
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(TONGDA_KICK_12M, VecPosition(15, 0, 0));
        }
        else return cy_findball(FORWARD_LEFT);
    }
    else if(me.getDistanceTo(ball)<3)
    {
        if(getPlayerClosestToBall()==worldModel->getUNum())
        {
            return kickBall(TONGDA_KICK_12M, VecPosition(15, 0, 0));
        }

        else return cy_findball(FORWARD_LEFT);
    }
    else if(posball.getX()>9&&posball.getY()<0&&posball.getY()>-10)
    {
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5)
        {
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        } 
        else
        {
             // not close to the target
             return goToSpace(VecPosition(10,3.5,0));
        }
      
    }
    else if(posball.getX()<-7)
    {
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5)
        {
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        } 
        else
        {
             // not close to the target
            return goToSpace(VecPosition(3,6,0));
        }
    }
    else
    {
        if (me.getDistanceTo(getSpaceForRole(agentRoles[worldModel->getUNum()])) < 0.5)
        {
             // close enough to turn to the ball
            return goToTargetRelative(VecPosition(), atan2Deg(worldModel->g2l(ball).getY(), worldModel->g2l(ball).getX()));
        } 
        else
        {
             // not close to the target
            return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
        }
    }
}




////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//判断是否有大于自身x位置的球员并返回球员的编号
double NaoBehavior::isAnyPlayerAheadOfBall()
 {
    VecPosition ballPos = worldModel->getBall();

    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i) {
        WorldObject* player = worldModel->getWorldObject(i);
        if (player && player->validPosition) {
            VecPosition playerPos = player->pos;

            if (playerPos.getX() > ballPos.getX()) {
                return i; // 有球员的 X 坐标大于球的 X 坐标
            }
        }
    }
    return 0; // 没有球员的 X 坐标大于球的 X 坐标
}
//    场地左侧（me.getX() < 7）：
    //     找到离球门最近的队友，并计算其与球门的距离。
    //     根据队友距离球门的距离，选择不同的传球方式。如果队友是当前机器人自己，则根据当前机器人与球门的距离来选择传球方式。
    //     传球的目标点是一个固定的位置（VecPosition(14, 1, 0)），并根据不同的情况选择不同的踢球方式（TONGDA_KICK_12M、KICK_8M、SHOOT_UT）。

    // 场地右侧（me.getX() >= 7）：
    //     根据当前机器人到场地中心的距离选择不同的传球方式。
    //     传球的目标点同样是一个固定位置（VecPosition(14, 0, 0)），并根据不同的情况选择不同的踢球方式（TONGDA_KICK_12M、KICK_LOW、KICK_8M、KICK_SHORT）。
SkillType NaoBehavior::pass() {
    if (me.getX() < 7) {
        double playerClosestToBox = 1;
        double closestDistanceToBox = 99999999;

        for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i) {
            VecPosition temp;
            int playerNum = i - WO_TEAMMATE1 + 1;

            if (worldModel->getUNum() == playerNum) {
                temp = worldModel->getMyPosition();
            } else {
                WorldObject *teammate = worldModel->getWorldObject(i);
                if (teammate->validPosition) {
                    temp = teammate->pos;
                } else {
                    continue;
                }
            }
            temp.setZ(0);

            double distanceToBox = temp.getDistanceTo(football_box_2);
            if (distanceToBox < closestDistanceToBox) {
                playerClosestToBox = playerNum;
                closestDistanceToBox = distanceToBox;
            }
        }
          
        VecPosition Be_passed_position = worldModel->getOpponent(playerClosestToBox);
        double pass_distance = distanceBetweenPoints(Be_passed_position, me);
 
        if (playerClosestToBox == worldModel->getUNum()) {
            if(closestDistanceToBox>=13)
            {
            return kickBall(TONGDA_KICK_12M , VecPosition(14, 1, 0));
            }
            else if(closestDistanceToBox<=13&&closestDistanceToBox>=9)
              {
                return kickBall(KICK_8M, VecPosition(14, 1, 0));
              }
            else
            {
                return kickBall(SHOOT_UT,VecPosition(14,1,0));
            }
        } 
        else 
        {
            if (pass_distance <= 9 || pass_distance > 11) {
                return kickBall(KICK_8M, VecPosition(Be_passed_position.getX() + 0.5, Be_passed_position.getY() - 0.5, 0));
            } else if (pass_distance <= 5) {
                return kickBall(KICK_IK, VecPosition(Be_passed_position.getX() + 0.5, Be_passed_position.getY() - 0.5, 0));
            } else {
                return kickBall(KICK_LOW, VecPosition(Be_passed_position.getX() + 0.5, Be_passed_position.getY() + 0.5, 0));
            }
        }
    } else {
        double distanceToCenter = me.getDistanceTo(VecPosition(0, 0, 0));
        
        if (distanceToCenter > 12) {
            return kickBall(TONGDA_KICK_12M, VecPosition(14, 0, 0));
        } else if (distanceToCenter > 10) {
            return kickBall(KICK_LOW, VecPosition(14, 0, 0));
        } else if (distanceToCenter > 7) {
            return kickBall(KICK_8M, VecPosition(14, 0, 0));
        } else {
            return kickBall(TONGDA_KICK_12M, VecPosition(15, 0, 0));
        }
    }
}


// if(me.getX() >= -15 && me.getY() >= -10&& me.getX() <= -6.5 && me.getY() < 10)
//      {
//         ifpassmode();
//         return kickBall(KICK_DRIBBLE, VecPosition(5, -8, 0)); 
//      }
//      if (me.getX() >= 3 && me.getY() >= 7&& me.getX() <= 13 && me.getY() < 10) 
//      {
//          if(me.getX()<=ball.getX())
//          {
//       if (me.getX()<=13&&me.getX()>=10)
//        {
//           ifpassmode();
//         //   return kickBall(TONGDA_KICK_12M, VecPosition(15, -1, 0));
//         return kickBall(KICK_DRIBBLE,VecPosition(13,7,0));
//        }
//        else
//        {
//         return kickBall(KICK_DRIBBLE, VecPosition(13, 7, 0));
//        }
//          }
//          else
//          {
//              return kickBall(KICK_IK,VecPosition(posball.getX()-0.5,posball.getY()-0.5,0));
//          }
//     }

//     else if (me.getX() >= 3 && me.getY() <= -7 && me.getX() <= 13 && me.getY() > -10)
//      {
//           if(me.getX()<=ball.getX())
//          {
//        if (me.getX()<=13&&me.getX()>=10)
//        {
//           getinpassmode=false;
//         return kickBall(TONGDA_KICK_12M, VecPosition(15, 1, 0));
//        }
//        else
//        {
//            std::cout << "带球" << std::endl;
//         return kickBall(KICK_DRIBBLE, VecPosition(13, -7, 0));
//        }
//          }
//          else
//          {
//             return kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//          }
//      }
//      else if (me.getX() >= 0.5 && me.getX() < 5 && me.getY() < 7 && me.getY() > -7) 
//      {
//          if(me.getX()<=ball.getX())
//          {
//          if (isAnyPlayerAheadOfBall()!=0)
//          {
//             getinpassmode = true;
//             return pass();
            
//         } else
//         {
//             if (me.getY() > 2.5 && me.getY() < 7&& me.getX() >= 0.5&& me.getX() <= 3) {
//                 return kickBall(KICK_DRIBBLE, VecPosition(13, 5, 0));
//             } else if (me.getY() < -2.5 && me.getY() > -7&& me.getX() >= 0.5 && me.getX() <= 3) {
//                 return kickBall(KICK_DRIBBLE, VecPosition(13, -5, 0));
//             } else if (me.getY() >= -2.5 && me.getY() <= 2.5 && me.getX() >= 0.5 && me.getX() <= 3) {
//                 return kickBall(TONGDA_KICK_12M,VecPosition(13,-7, 0));
//             }
//         }
//      }
//      else
//      {
//          return kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//      }
//    }
//      else if (me.getX() >=5 && me.getX() <= 8 && me.getY() <7&& me.getY() > -7)
//       {
//         if(me.getX()<=ball.getX())
//         {
//             if(me.getX()>=5&&me.getX()<=7)
//            {
//              if(me.getY()<=2.5&&me.getY()>=-2.5)
//              {
//                  getinpassmode = true;
//                 return pass();
//              }
//              else if(me.getY()>2.5&&me.getY()<7)
//              {
//                return kickBall(KICK_DRIBBLE,VecPosition(7.5,5,0));
//              }
//              else if(me.getY()<-2.5&&me.getY()>-7)
//              {
//                 return kickBall(KICK_DRIBBLE,VecPosition(7.5,-5,0));
//              }
//            }
//            else if(me.getX()>7&&me.getX()<=8)
//            {
//              if(me.getY()<=2.5&&me.getY()>=-2.5)
//              {
//                  return pass();
//              }
//              else if(me.getY()>2.5&&me.getY()<7)
//              {   
//                return shoot();
//              }
//              else if(me.getY()<-2.5&&me.getY()>-7)
//              { 
//                 return shoot();
//              }
//            }
//         }
//         else
//         {
//              return kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//         }
//       }
//      else if (me.getX()>8&&me.getX()<11&&me.getY()<7&&me.getY()>-7)
//      {   if(me.getX()<=ball.getX())
//         {
//          if (me.getY() > 2.5 && me.getY() < 7) 
//          {    getinpassmode=false;
//                 return kickBall(SHOOT_UT,VecPosition(15,0, 0));
//         }
//              else if (me.getY() < -2.5 && me.getY() > -7 ) 
//             {getinpassmode=false;
//                 return kickBall(TONGDA_KICK_12M,VecPosition(15,0, 0));
//             } 
//             else if (me.getY() >= -2.5 && me.getY() <= 2.5 ) {getinpassmode=false;
//                 return kickBall(SHOOT_UT,VecPosition(15,0, 0));
//             }
//         }
//         else 
//         {
//              return kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//         }
//      }
//      else if(me.getX()>=11&&me.getX()<13&&me.getY()<7&&me.getY()>-7)
//      {
//         if(me.getX()<=ball.getX())
//         {
//             if(me.getY()>=2.5&&me.getY()<7)
//             {    getinpassmode=false;
//                 return shoot();
//             }
//             else if(me.getY()<2.5&&me.getY()>-2.5)
//             {getinpassmode=false;
//                 return kickBall(KICK_IK,getRolePosition(MID_LEFT));
//             }
//             else if(me.getY()<=-2.5&&me.getY()>-7)
//             {    getinpassmode=false;
//                 return kickBall(SHOOT_UT,VecPosition(15,0, 0));
//             }
//         }
//         else
//         {
//              return kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//         }
//      }
//      else if (me.getY() < 10 && me.getY() >= 5 && me.getX() >13 && me.getX() < 15)
//       {
//         if(me.getX()<=ball.getX()){
//         // return kickBall(TONGDA_KICK_12M, getRolePosition(WING_LEFT).getX()+0.5);
//        if(getRolePosition(MID_LEFT).getX()>me.getX())
//        {
//         if(distanceBetweenPoints(me,getRolePosition(MID_LEFT).getX())<=9)
//         {
//              return kickBall(KICK_8M,getRolePosition(MID_LEFT).getX()+0.5);
//         }else
//         {
//               return kickBall(SHOOT_UT,getRolePosition(MID_LEFT).getX()+0.5);
//         }
//        }
//         }
//        else
//        {
//         return  kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//        }
//     } 
//      else if (me.getX() > 13 && me.getX() < 15 && me.getY() <= -5 && me.getY() > -10)
//      {
//         if(me.getX()<=ball.getX())
//         {
//         // return kickBall(TONGDA_KICK_12M, getRolePosition(WING_LEFT).getX()+0.5);
//        if(getRolePosition(MID_LEFT).getX()>me.getX())
//        {
//         if(distanceBetweenPoints(me,getRolePosition(MID_LEFT).getX())<=9)
//         {
//              return kickBall(SHOOT_UT,getRolePosition(MID_LEFT).getX()+0.5);
//         }else
//         {
//               return kickBall(SHOOT_UT,getRolePosition(MID_LEFT).getX()+0.5);
//         }
//        }
//        else
//        {
//         return  kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//        }
//         }
//         else
//         {
//             return  kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//         }
//      }
//     else if (me.getX()>=0.5&&me.getX()<3&&me.getY()<10&&me.getY()>=7)
//     {
//       if(isOurTeammateWithBall())
//     {
//         ifpassmode();
//        if(getinpassmode)
//        {
//         // print2();
//        return pass();
//        }
//        else
//        {
//         // print1();
//         return kickDribbleAvoid(VecPosition(13,-7,0));
//        }
//     }
//     else
//     {
//          return kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//     }
//     }
//      else if (me.getX()>=0.5&&me.getX()<3&&me.getY()>-10&&me.getY()<=-7)
//     { 
//         if(isOurTeammateWithBall())
//         {
//        ifpassmode();
//        if(getinpassmode)
//        {
//         //  print2();
//        return pass();
//        }
//        else
//        {
//         //   print1();
//         return kickDribbleAvoid(VecPosition(13,-7,0));
//        }
//         }
//         else
//     {
//          return kickBall(KICK_IK,VecPosition(posball.getX()+0.5,posball.getY()+0.5,0));
//     }
//     }
//     else if(me.getX()>=13&&me.getX()<15&&me.getY()>-5&&me.getY()<5)
//     {
//          if(me.getY()>=2.5&&me.getY()<5&&me.getX()>=14&&me.getX()<15)
//          {
//             return kickBall(KICK_IK,VecPosition(14.5,0,0));
//          }
//         if(me.getY()<-2.5&&me.getY()>-5&&me.getX()>=14&&me.getX()<15)
//          {
//             return kickBall(KICK_IK,VecPosition(14.5,0,0));
//          }
//         else if(me.getY()>0)
//         {    getinpassmode=false;
//             return kickBall(KICK_DRIBBLE, VecPosition(15, 0.5, 0));
//         }
//         else if (me.getY()<=0)
//         {    getinpassmode=false;
//             return kickBall(KICK_DRIBBLE, VecPosition(15, 0.5, 0));
//         }
//     }
//     else if(me.getX()<0.5&&me.getX()>=-2&&me.getY()<=0&&me.getY()>-10)
//     {
//         ifpassmode();
//        if(getinpassmode)
//        {
//         //  print2();
//        int  opp_less_place=calculateZoneDensities();
//     if(opp_less_place==1||opp_less_place==2)       
//     {
//          return kickBall(TONGDA_KICK_12M, VecPosition(5, 6, 0));
//     }           
//     else if(opp_less_place==3||opp_less_place==4) 
//     {
//          return kickBall(TONGDA_KICK_12M, VecPosition(5, -6, 0));
//     }   
//        }
//        else
//        {
//         //   print1();
//         return kickDribbleAvoid(VecPosition(1,-7,0));
//        }                                                                                        
//     }
//      else if(me.getX()<0.5&&me.getX()>=-2&&me.getY()>0&&me.getY()<10)
//     {

//         ifpassmode();
//        if(getinpassmode)
//        {
//        int  opp_less_place=calculateZoneDensities();
//     if(opp_less_place==1||opp_less_place==2)       
//     {
//          return kickBall(TONGDA_KICK_12M, VecPosition(5, 6, 0));
//     }           
//     else if(opp_less_place==3||opp_less_place==4) 
//     {
//          return kickBall(TONGDA_KICK_12M, VecPosition(5, -6, 0));
//     }  
//        }
//           else
//        {
//         // print1();
//         return kickDribbleAvoid(VecPosition(1,7,0));
//        }  
//     } 
//     else
//     {

//         return kickBall(KICK_DRIBBLE, VecPosition(15, 0, 0));
//     }