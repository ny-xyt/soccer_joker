#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"
#include <algorithm>
#include <sstream>
#include <bits/stdc++.h>

extern int agentBodyType;

who_close who_close_t;
SkillType NaoBehavior::attack_time()
 {

    //updateSatisfaction();
   int i=calculateZoneDensities();
//    std::cout <<  i << std::endl;

        agentRoles = KM_RoleAssign(); 
        VecPosition space = getSpaceForRole(agentRoles[worldModel->getUNum()]);//获取当前球员分配的角色对应的空间位置，以便为球员制定适当的行为和策略
       

    mdrawTargetPosition(); //在RoboViz中显示每个球员的目标位置和角色信息
      VecPosition posball = worldModel->getBall();
    mdrawRoles();

    // if(worldModel->getPlayMode() != PM_PLAY_ON){
    //     return judegment_playmode();
    // }
    if (agentRoles[worldModel->getUNum()] == GOALIE) {
        //    print1( GOALIE);
        return GOALIE_Tactic();
    }
    else if (agentRoles[worldModel->getUNum()] == HELPER) {
       
        return HELPER_Tactic(); 
    }
    else if (agentRoles[worldModel->getUNum()] == BACK_LEFT) {
             
        return BACK_LEFT_Tactic();
    }
    else if (agentRoles[worldModel->getUNum()] == BACK_RIGHT) {
//  print1( BACK_RIGHT);
        return BACK_RIGHT_Tactic();
    }
    else if (agentRoles[worldModel->getUNum()] ==  BACK_MID)
    {
    //  print1( BACK_MID);
        return BACK_MID_Tactic();
    }
    else if(agentRoles[worldModel->getUNum()] == MID_LEFT)
    {
     
        return MID_LEFT_Tactic();
     
    }
    else if (agentRoles[worldModel->getUNum()] == MID_RIGHT)
    {
   return MID_RIGHT_Tactic();
    }

    else if ( agentRoles[worldModel->getUNum()] == MID_MID)
    {
        return MID_MID_Tactic();

    }
    else if( agentRoles[worldModel->getUNum()] == FORWARD_MID)
    {
    return FORWARD_MID_Tactic();
    }
    else if(agentRoles[worldModel->getUNum()] == FORWARD_LEFT) 
    {
        // print1(FORWARD_LEFT);
      return FORWARD_LEFT_Tactic();
    }
   else if(agentRoles[worldModel->getUNum()] == FORWARD_RIGHT)
          {  

//       print1( FORWARD_RIGHT);
           return FORWARD_RIGHT_Tactic();
            }
    else
   {
    return  goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
   }
}


////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////



SkillType NaoBehavior::selectSkill() 
{

//   if(worldModel->getPlayMode() != PM_PLAY_ON)
//     {
//         return judegment_playmode();
//     }
 
    VecPosition posball = worldModel->getBall();
   int  opp_less_place=calculateZoneDensities();
 if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
 { 
    if(opp_less_place==1||opp_less_place==2)       
    {
         if (worldModel->getUNum()==9) 
                 {
                  return kickBall(KICK_8M,VecPosition(-6,-7 , 0) );
                }
         else
               {
                return SKILL_STAND;
                }
    }        
    else if(opp_less_place==3||opp_less_place==4)   
    {
         if (worldModel->getUNum()==9) 
                {   
     
                 return kickBall(KICK_8M,VecPosition(-6,7 , 0) );
                }
        else
                 {
                return SKILL_STAND;
                }  
    }
    else
   {
                 if (worldModel->getUNum()==9) 
                 {
                return kickBall(KICK_8M,VecPosition(-6,-7 , 0) );
                 }
                else
                {
                  return SKILL_STAND;
                }
    }
 }
 else
   {
    return attack_time();
   }
}

SkillType NaoBehavior::start_ball()
{
  VecPosition posball = worldModel->getBall();
    VecPosition target_ball=VecPosition(posball.getX(), 0, 0);
        
    WorldObject* getwo6 = worldModel->getWorldObject(WO_TEAMMATE6);
    WorldObject* getwo10 = worldModel->getWorldObject(WO_TEAMMATE10);
    WorldObject* getwo11 = worldModel->getWorldObject(WO_TEAMMATE11);
    VecPosition pos6, pos10, pos11;
    pos6= getwo6->pos;
    pos10 = getwo10->pos;
    pos11 = getwo11->pos;
    target_ball = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target_ball, false /*fKeepDistance*/);
    go_back();
   
 // who_close_to_ball();

    if (worldModel->getUNum()==9) 
    {
     
        if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
        {
            // VecPosition pos3=worldModel->getOpponent(3);
            return kickBall(KICK_8M,VecPosition(15,0 , 0) );
        }
        if(getPlayerClosestToBall()==9)
         {
            return kickBall(TONGDA_KICK_12M,VecPosition(8, -6, 0));
         }
        if(posball.getX()<2&&posball.getX()>-1&&posball.getY()<-1&&posball.getY()>-6)
        {
            if(getPlayerClosestToBall()==9)
            {
                    return kickBall(KICK_8M, VecPosition(8, -7, 0));
            }
            else
            {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.5, posball.getY(), 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);   
            }
        }
        // if(posball.getX()<-1&&posball.getX()>-5&&posball.getY()<-1&&posball.getY()>-6)
        // {
        //     if(getPlayerClosestToBall()==9)
        //     {
        //             return kickBall(KICK_8M, VecPosition(8, -7, 0));
        //     }
        //     else
        //     {
        //             VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.6, posball.getY()+0.6, 0));
		//             target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
        //             return goToTarget(target5);   
        //     }
        // }
        else
        {
                VecPosition target5 = VecPosition(VecPosition(4, 1.5, 0));
		        target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                return goToTarget(target5);                     
    
        }

    }
    if (worldModel->getUNum()==7) 
    {
            if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
            {
                return SKILL_STAND;
            }
            if(getPlayerClosestToBall()==7)
         {
            return kickBall(TONGDA_KICK_12M, VecPosition(8, -6, 0));
         }
            // if(posball.getX()>-7&&posball.getX()<2&&posball.getY()>-10&&posball.getY()<-5) 
            // {
            //      VecPosition target5 = VecPosition(VecPosition(-10, -3, 0));
		    //      target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
            //      return goToTarget(target5); 
            // }
            // if(posball.getX()>-7&&posball.getX()<2&&posball.getY()>5&&posball.getY()<10) 
            // {
            //      VecPosition target5 = VecPosition(VecPosition(-10, 3, 0));
		    //      target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
            //      return goToTarget(target5); 
            // }
            if(posball.getX()>-7&&posball.getX()<-4&&posball.getY()>0&&posball.getY()<5) 
            {
                if(getPlayerClosestToBall()==7)
                  {
                    return kickBall(TONGDA_KICK_12M, VecPosition(8, -7, 0));
                  } 
                  else
                  {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()-1, posball.getY()-0.5, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);  
                  }
            }
            if(posball.getX()>-7&&posball.getX()<-4&&posball.getY()>-5&&posball.getY()<0) 
            {
                if(getPlayerClosestToBall()==7)
                  {
                    return kickBall(TONGDA_KICK_12M, VecPosition(8, -7, 0));
                  } 
                  else
                  {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()-1, posball.getY()+0.5, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);  
                  }
            }
            else 
            {
                 VecPosition target5 = VecPosition(VecPosition(-8, 0, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            } 
    }
    if (worldModel->getUNum()==1) 
    {
        if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
            {
                return SKILL_STAND;
            }
            if(getPlayerClosestToBall()==1)
         {
            return kickBall(TONGDA_KICK_12M, VecPosition(8, -6, 0));
         }
         if(posball.getX()>-7&&posball.getX()<2&&posball.getY()>-10&&posball.getY()<-5) 
            {
                 VecPosition target5 = VecPosition(VecPosition(-14.5, -1.5, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            }
            if(posball.getX()>-7&&posball.getX()<2&&posball.getY()>5&&posball.getY()<10) 
            {
                 VecPosition target5 = VecPosition(VecPosition(-14.5, 1.5, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            }
            else 
            {
                 VecPosition target5 = VecPosition(VecPosition(-14.5, 0, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            } 
    }
    if (worldModel->getUNum()==2)
    {
            if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
            {
                return SKILL_STAND;
            }
            if(getPlayerClosestToBall()==2)
         {
            return kickBall(TONGDA_KICK_12M, VecPosition(8, -6, 0));
         }
            if(posball.getX()>-7&&posball.getX()<-4&&posball.getY()>0&&posball.getY()<10) 
            {
                if(getPlayerClosestToBall()==2)
                  {
                    return kickBall(TONGDA_KICK_12M, VecPosition(8, -7, 0));
                  } 
                  else
                  {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()-1, posball.getY(), 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);  
                  }
            }
            // if(posball.getX()>-7&&posball.getX()<2&&posball.getY()>-10&&posball.getY()<-5) 
            // {
            //      VecPosition target5 = VecPosition(VecPosition(-8, 1, 0));
		    //      target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
            //      return goToTarget(target5); 
            // }
            // if(posball.getX()>-7&&posball.getX()<2&&posball.getY()>5&&posball.getY()<10) 
            // {
            //      VecPosition target5 = VecPosition(VecPosition(-12, 5, 0));
		    //      target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
            //      return goToTarget(target5); 
            // }
            else 
            {
                 VecPosition target5 = VecPosition(VecPosition(-8, 3, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            } 
    
    } 
    if (worldModel->getUNum()==3)
    {
           if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
           {
                 return SKILL_STAND;
           }
           if(getPlayerClosestToBall()==3)
         {
            return kickBall(TONGDA_KICK_12M, VecPosition(8, -6, 0));
         }
     
           if (posball.getX()<0&&posball.getX()>-8&&posball.getY()<-3&&posball.getY()>-7) 
           {
                  if(getPlayerClosestToBall()==3)
                  {
                    return kickBall(TONGDA_KICK_12M, VecPosition(8, -7, 0));
                  } 
                  else
                  {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.6, posball.getY()-0.5, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);  
                  }

            }
            if (posball.getX()<0&&posball.getX()>-8&&posball.getY()<-7) 
           {
                  if(getPlayerClosestToBall()==3)
                  {
                    return kickBall(TONGDA_KICK_12M, VecPosition(8, -7, 0));
                  } 
                  else
                  {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.6, posball.getY(), 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);  
                  }

            }
            else
            {
                    VecPosition target5 = VecPosition(-5, -7, 0);
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);     
            }

    } 
    if (worldModel->getUNum()==4)
    {

     
            if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
            {
                return SKILL_STAND;
            }
            if(getPlayerClosestToBall()==4)
         {
            return kickBall(TONGDA_KICK_12M, VecPosition(8, -6, 0));
         }

            if(posball.getX()>-7&&posball.getX()<-4&&posball.getY()>-10&&posball.getY()<0) 
            {
                if(getPlayerClosestToBall()==4)
                  {
                    return kickBall(TONGDA_KICK_12M, VecPosition(8, -7, 0));
                  } 
                  else
                  {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()-1, posball.getY(), 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);  
                  }
            }
            // if(posball.getX()>-7&&posball.getX()<2&&posball.getY()>5&&posball.getY()<10) 
            // {
            //      VecPosition target5 = VecPosition(VecPosition(-8, -1, 0));
		    //      target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
            //      return goToTarget(target5); 
            // }
            else 
            {
                 VecPosition target5 = VecPosition(VecPosition(-8, -3, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            }   
    } 
    if (worldModel->getUNum()==5)
    {
        if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
            {
                return SKILL_STAND;
            }
            if(getPlayerClosestToBall()==5)
         {
           return kickBall(TONGDA_KICK_12M, VecPosition(8, -6, 0));
         }
        if (posball.getX()<0&&posball.getX()>-4&&posball.getY()>-5&&posball.getY()<5)
        {

            if(getPlayerClosestToBall()==5)
            {
                    return kickBall(TONGDA_KICK_12M, VecPosition(8, -7, 0));
            }
            else
            {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()+1, posball.getY(), 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);   
            }
        }
        if (posball.getX()<-4&&posball.getX()>-8&&posball.getY()>-5&&posball.getY()<5)
        {

            if(getPlayerClosestToBall()==5)
            {
                    return kickBall(TONGDA_KICK_12M, VecPosition(8, -7, 0));
            }
            else
            {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()-1, posball.getY()+0.4, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);   
            }
        }
        else
        {
                VecPosition target5 = VecPosition(VecPosition(-4, 0, 0));
		        target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                return goToTarget(target5);                     
    
        }


    } 
    if (worldModel->getUNum()==6)
    {
        if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
            {
                return SKILL_STAND;
            }
            if(getPlayerClosestToBall()==6)
         {
            return kickBall(TONGDA_KICK_12M, VecPosition(8, -6, 0));
         }
   
                 if (posball.getX()<2&&posball.getX()>-4&&posball.getY()>2)
                 {

                    if(getPlayerClosestToBall()==6)
                    {
                        return kickBall(TONGDA_KICK_12M, VecPosition(8, -7, 0));
                    }
                    else
                    {
                        VecPosition target5 = VecPosition(VecPosition(posball.getX()-1, posball.getY(), 0));
		                target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                        return goToTarget(target5);   
                    }
                 }
                 if (posball.getX()<-4&&posball.getX()>-7&&posball.getY()>2)
                 {

                    if(getPlayerClosestToBall()==6)
                    {
                        return kickBall(TONGDA_KICK_12M, VecPosition(8, -7, 0));
                    }
                    else
                    {
                        VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.5, posball.getY(), 0));
		                target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                        return goToTarget(target5);   
                    }
                 }
                 else
                 {
                     VecPosition target5 = VecPosition(VecPosition(-4, 3.5, 0));
		             target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                     return goToTarget(target5);                     
    
                 }
            
    } 
    if (worldModel->getUNum()==8)
    {
                if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
            {
                return SKILL_STAND;
            }
            if(getPlayerClosestToBall()==8)
         {
            return kickBall(TONGDA_KICK_12M, VecPosition(8, -6, 0));
         }

                 if (posball.getX()<2&&posball.getX()>-5&&posball.getY()>1)
                 {

                    if(getPlayerClosestToBall()==8)
                    {
                        return kickBall(KICK_8M, VecPosition(8, -7, 0));
                    }
                    else
                    {
                        VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.5, posball.getY(), 0));
		                target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                        return goToTarget(target5);   
                    }
                 }
                //  if (posball.getX()<2&&posball.getX()>-5&&posball.getY()>6)
                //  {

                //     if(getPlayerClosestToBall()==8)
                //     {
                //         return kickBall(TONGDA_KICK_12M, VecPosition(-2, -5, 0));
                //     }
                //     else
                //     {
                //         VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.5, posball.getY()+1, 0));
		        //         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                //         return goToTarget(target5);   
                //     }
                //  }
                 else
                 {
                     VecPosition target5 = VecPosition(VecPosition(1, 0, 0));
		             target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                     return goToTarget(target5);                     
    
                 }
    
    }
    if (worldModel->getUNum()==10)
    {     
         if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
         {
             return SKILL_STAND;
         }
         if(getPlayerClosestToBall()==10)
         {
            return kickBall(TONGDA_KICK_12M, VecPosition(8, -6, 0));
         }
        // if (posball.getX()<0.5&&posball.getX()>-2&&posball.getY()>-10&&posball.getY()<-5)
        // {

        //     if(getPlayerClosestToBall()==10)
        //     {
        //             return kickBall(KICK_8M, VecPosition(8, -7, 0));
        //     }
        //     else
        //     {
        //             VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.5, posball.getY(), 0));
		//             target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
        //             return goToTarget(target5);   
        //     }
        // }
        // if (posball.getX()<-2&&posball.getX()>-5&&posball.getY()>-10&&posball.getY()<-5)
        // {

        //     if(getPlayerClosestToBall()==10)
        //     {
        //             return kickBall(TONGDA_KICK_12M, VecPosition(8, -7, 0));
        //     }
        //     else
        //     {
        //             VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.5, posball.getY()+0.5, 0));
		//             target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
        //             return goToTarget(target5);   
        //     }
        // }
        else
        {
                VecPosition target5 = VecPosition(VecPosition( 4.5, -5, 0));
		        target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                return goToTarget(target5);                     
    
        }

    } 
    if (worldModel->getUNum()==11)
    {
        if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
         {
             return SKILL_STAND;
         }
         if(getPlayerClosestToBall()==11)
         {
          return kickBall(TONGDA_KICK_12M, VecPosition(15, 0, 0));
         }
        // if (posball.getX()<2.5&&posball.getX()>-4&&posball.getY()<-4)
        // {

        //     if(getPlayerClosestToBall()==11)
        //     {
        //             return kickBall(TONGDA_KICK_12M, VecPosition(7, 2, 0));
        //     }
        //     else
        //     {
        //             VecPosition target5 = VecPosition(VecPosition(posball.getX()+1.5, posball.getY()+1.5, 0));
		//             target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
        //             return goToTarget(target5);   
        //     }
        // }
        if(me.getX()>8)
        {
            return goToTarget(VecPosition(8,-8,0));
        }
        else
        {
                // VecPosition target5 = VecPosition(VecPosition(7, -7, 0));
		        // target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                // return goToTarget(target5);                     
                 return goToTarget(VecPosition(me.getX()+1,me.getY()+0.45,0));                     
                
        }
    } 
 }
////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
SkillType NaoBehavior::defend()
{
   if((worldModel->getPlayMode() == PM_GOAL_KICK_LEFT && worldModel->getSide() == SIDE_LEFT)
             || (worldModel->getPlayMode() == PM_GOAL_KICK_RIGHT && worldModel->getSide() == SIDE_RIGHT)) 
             {
                return  kickBall(TONGDA_KICK_12M,VecPosition(0,-7,0));
             }
             else 
             {
    VecPosition posball = worldModel->getBall();
    VecPosition target_ball=VecPosition(posball.getX(), 0, 0);
    target_ball = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target_ball, false /*fKeepDistance*/);
        //defend_time==true;
        
        if(worldModel->getUNum() == 1)
        {
 
           
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
            else 
            {
                 VecPosition target5 = VecPosition(VecPosition(-14.5, 0, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            }  
        }
      /////////////////////////////////////////////2号球员防上面
        if(worldModel->getUNum() == 2) 
        {    

            if(posball.getX()==-15&&posball.getY()>0)
            {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()+1, posball.getY()-0.5, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);   
                           
            }
            if(posball.getX()>-9&&posball.getX()<-7&&posball.getY()>0)
            {
                if(getPlayerClosestToBall()==2)
                {
                    return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                }  
                else 
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.4, posball.getY()+0.2, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);               
                } 
            }    
            if(posball.getX()>=-12.5&&posball.getX()<-9&&posball.getY()>0)
            {
                 if(getPlayerClosestToBall()==2)
                {
                    return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                }  
                else 
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.3, posball.getY()+0.3, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);               
                }    
            }                  
            if(posball.getX()<-12.5)
            {
              if(posball.getY()>0)
              {
                if(getPlayerClosestToBall()==2)
                {
                    return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                }  
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()+0.4, posball.getY()+0.3, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                }
              }
              if(posball.getY()<0&&posball.getY()>-3)
              {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()+2, posball.getY()+2, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
              }   
            }
            if(posball.getX()<-7&&posball.getY()>10)
            {
                    VecPosition target5 = VecPosition(VecPosition(-11, 5, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);                  
            }
            if(posball.getX()>-15&&posball.getX()<-7&&posball.getY()>-10&&posball.getY()<-5) 
            {
                 VecPosition target5 = VecPosition(VecPosition(-8, 1, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            }
            if(posball.getX()>-15&&posball.getX()<-7&&posball.getY()>5&&posball.getY()<10) 
            {
                 VecPosition target5 = VecPosition(VecPosition(-12, 5, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            }
            else 
            {
                 VecPosition target5 = VecPosition(VecPosition(-10, 3, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            } 
        } 
        ///////////////////////////////////////////7号球员防中间
        if(worldModel->getUNum() == 7) 
        { 
            if(posball.getX()==15&&posball.getX()>0)
            {
                    VecPosition target5 = VecPosition(VecPosition(-14.5, 2.5, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);                    
            }
            if(posball.getX()==15&&posball.getX()<0)
            {
                    VecPosition target5 = VecPosition(VecPosition(-14.5, -2.5, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);                    
            }
            if(posball.getX()>-10&&posball.getX()<-7&&posball.getY()<5&&posball.getY()>0)
            {
                    if(getPlayerClosestToBall()==7)
                    {
                       return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                    }  
                    else
                    {
                        VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.4, posball.getY()-0.1, 0));
		                target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                        return goToTarget(target5);    
                    } 
            }
            if(posball.getX()>-13&&posball.getX()<-10&&posball.getY()<5&&posball.getY()>0)
            {
                    if(getPlayerClosestToBall()==7)
                    {
                       return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                    }  
                    else
                    {
                        VecPosition target5 = VecPosition(VecPosition(posball.getX()+0.3, posball.getY()-1, 0));
		                target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                        return goToTarget(target5);    
                    } 
            }
            if(posball.getX()>-15&&posball.getX()<-13&&posball.getY()<5&&posball.getY()>0)
            {
                    if(getPlayerClosestToBall()==7)
                    {
                       return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                    }  
                    else
                    {
                        VecPosition target5 = VecPosition(VecPosition(posball.getX()+0.2, posball.getY()-1, 0));
		                target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                        return goToTarget(target5);    
                    } 
            }
            
            if(posball.getX()>-10&&posball.getX()<-7&&posball.getY()<0&&posball.getY()>-5)
            {
                    if(getPlayerClosestToBall()==7)
                    {
                      return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                    }  
                    else
                    {
                        VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.3, posball.getY()+0.5, 0));
		                target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                        return goToTarget(target5);    
                    } 
            }
            
            if(posball.getX()>-15&&posball.getX()<-10&&posball.getY()<0&&posball.getY()>-5)
            {
                    if(getPlayerClosestToBall()==7)
                    {
                       return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                    }  
                    else
                    {
                        VecPosition target5 = VecPosition(VecPosition(posball.getX()+0.4, posball.getY()+0.4, 0));
		                target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                        return goToTarget(target5);    
                    } 
            }
            if(posball.getY()>-10&&posball.getY()<-5) 
            {
                 VecPosition target5 = VecPosition(VecPosition(-10, -3, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            }
            if(posball.getY()>5&&posball.getY()<10) 
            {
                 VecPosition target5 = VecPosition(VecPosition(-10, 3, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            }
            else 
            {
                 VecPosition target5 = VecPosition(VecPosition(-10, 0, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            } 
        }
   /////////////////////////////////////////4号球员防下面
         if(worldModel->getUNum() == 4) 
        {    

            if(posball.getX()==15&&posball.getY()<0)
            {
               
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()+1, posball.getY()+0.5, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                }
            }
            if(posball.getX()>-9&&posball.getX()<-7&&posball.getY()<0&&posball.getY()>-7)
            {
                if(getPlayerClosestToBall()==4)
                {
                   return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                }  
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX(), posball.getY(), 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                }
            }    
            if(posball.getX()>-13&&posball.getX()<-9&&posball.getY()<0&&posball.getY()>-7)
            {
                if(getPlayerClosestToBall()==4)
                {
                    return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                }  
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.4, posball.getY()-0.4, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                }  
            }
            if(posball.getX()>-9&&posball.getX()<-7&&posball.getY()<-7&&posball.getY()>-10)
            {
                if(getPlayerClosestToBall()==4)
                {
                    return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                }  
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.5, posball.getY(), 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                }
            }    
            if(posball.getX()>-13&&posball.getX()<-9&&posball.getY()<-7&&posball.getY()>-10)
            {
                if(getPlayerClosestToBall()==4)
                {
                    return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                }  
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.3, posball.getY()-0.3, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                }   
            }                  
            if(posball.getX()<-13)
            {
              if(posball.getY()<0)
              {
                if(getPlayerClosestToBall()==4)
                {
                    return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                }  
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX(), posball.getY()-0.4, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                }
              }
              if(posball.getY()<3&&posball.getY()>0) 
              {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()+2, posball.getY()-3, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
              }   
            }
            if(posball.getX()<-7&&posball.getY()<-10)
            {
                    VecPosition target5 = VecPosition(VecPosition(-11, -5, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
            }
            if(posball.getX()>-15&&posball.getX()<-7&&posball.getY()>-10&&posball.getY()<-5) 
            {
                 VecPosition target5 = VecPosition(VecPosition(-12, -5, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            }
            if(posball.getX()>-15&&posball.getX()<-7&&posball.getY()>5&&posball.getY()<10) 
            {
                 VecPosition target5 = VecPosition(VecPosition(-8, -1, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            }
            else 
            {
                 VecPosition target5 = VecPosition(VecPosition(-10, -3, 0));
		         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                 return goToTarget(target5); 
            } 
        }        
    /////////////////////////////////////////////////////////////////////////////
        if(worldModel->getUNum() == 5)
        { 
            if(posball.getX()>-12.5&&posball.getX()<-9&&posball.getY()>0&&posball.getY()<5) 
            {
                if(getPlayerClosestToBall()==5)
                {
                    return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                }  
                else 
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()+1.5, posball.getY()-0.2, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                } 

            }
            if(posball.getX()>-9&&posball.getX()<-6&&posball.getY()>0&&posball.getY()<5) 
            {
                if(getPlayerClosestToBall()==5)
                {
                     return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                }  
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()+0.5, posball.getY()+0.1, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                } 

            }
            if(posball.getX()>-12.5&&posball.getX()<-9&&posball.getY()>-5&&posball.getY()<0) 
            {
                if(getPlayerClosestToBall()==5)
                {
                    return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                }  
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()+1.5, posball.getY()+0.2, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                } 

            }
           if(posball.getX()>-9&&posball.getX()<-6&&posball.getY()>-5&&posball.getY()<0) 
            {
                if(getPlayerClosestToBall()==5)
                {
                      return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                }  
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()+0.5, posball.getY()+0.2, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);                    
                }
            }
            else
            {
                    VecPosition target5 = VecPosition(VecPosition(-6.5, 0, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
            }
        }
        //////////////////////////////////////////////////////////////////////////////
        if(worldModel->getUNum() == 6)
        {
            // if(posball.getX()>-10&&posball.getX()<-6&&posball.getY()>1&&posball.getY()<10) 
            // {
            if(posball.getX()>-10&&posball.getX()<-7&&posball.getY()>3)
            {
                if(getPlayerClosestToBall()==6)
                {
                    return kickBall(KICK_8M, VecPosition(0, -7, 0));
                }
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()+1, posball.getY(), 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                } 
            }
            else
            {
                VecPosition target5 = VecPosition(VecPosition(-7, 5, 0));
		        target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                return goToTarget(target5);   

            }
            // }
            // if(posball.getX()>-6&&posball.getX()<0&&posball.getY()>1&&posball.getY()<10) 
            // {
            //     if(getPlayerClosestToBall()==6)
            //     {
            //         return kickBall(TONGDA_KICK_12M, VecPosition(7, 0, 0));
            //     }
            //     else
            //     {
            //         VecPosition target5 = VecPosition(VecPosition(posball.getX(), posball.getY(), 0));
		    //         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
            //         return goToTarget(target5);                     
            //     } 
            // }
            // else
            // {
            //         VecPosition target5 = VecPosition(VecPosition(-4, 5, 0));
		    //         target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
            //         return goToTarget(target5); 
            // } 
        }
        //////////////////////////////////////////////////////////////////////////////
        if(worldModel->getUNum() == 3)
        {
            if(posball.getX()>-15&&posball.getX()<-12.5&&posball.getY()<-3&&posball.getY()>-10) 
            {
                if(getPlayerClosestToBall()==3)
                {
                      return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                }
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()+1, posball.getY()-1, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                } 
            }
            if(posball.getX()>-12.5&&posball.getX()<-6&&posball.getY()<-1&&posball.getY()>-10) 
            {
                if(getPlayerClosestToBall()==3)
                {
                     return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                }
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()+0.4, posball.getY()-0.5, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                }
            }
            else
            {
                    VecPosition target5 = VecPosition(VecPosition(-6, -4, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
            }     

        }
        if(worldModel->getUNum() == 8)
        {
                if(getPlayerClosestToBall()==8)
                {
                      return kickBall(TONGDA_KICK_12M, VecPosition(15, -7, 0));
                }
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(-2.5, 7, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                }
        }   
        if(worldModel->getUNum() == 9)
        {
                if(posball.getX()<0.1&&posball.getX()>-0.1&&posball.getY()<0.1&&posball.getY()>-0.1)
                {
                      return SKILL_STAND;
                }

                if(getPlayerClosestToBall()==9)
                {
                    return kickBall(KICK_8M, VecPosition(15, 0, 0));
                }
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(-2, 0, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                }
        }
        if(worldModel->getUNum() == 10)
        {
            if(posball.getX()>-12&&posball.getX()<-10&&posball.getY()<0)
            {
                if(getPlayerClosestToBall()==10)
                {
                      return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                }
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()+4.5, posball.getY()-2.5, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                } 
            }
            if(posball.getX()>-10&&posball.getX()<-7&&posball.getY()<0)
            {
                if(getPlayerClosestToBall()==10)
                {
                      return kickBall(TONGDA_KICK_12M, VecPosition(0, -7, 0));
                }
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()+2.5, posball.getY()-1, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                } 
            }
            else
            {
                VecPosition target5 = VecPosition(VecPosition(-5, -5, 0));
		        target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                return goToTarget(target5);   

            }
        } 
        if(worldModel->getUNum() == 11)
        {
                if(getPlayerClosestToBall()==11)
                {
                    return kickBall(KICK_8M, VecPosition(15, 0, 0));
                }
                else
                {
                    VecPosition target5 = VecPosition(VecPosition(3, -7.5, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5); 
                }
        }   
  

} 

}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

SkillType NaoBehavior::go_back()
{ // debug-success
    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i)
    { // WO_TEAMMATE1=1
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum)
        {
            temp = worldModel->getMyPosition();
            if (fabs(temp.getX()) > 15 && fabs(temp.getY()) > 10)
            {
                return goToTarget(VecPosition(0, 0, 0));
            }
        }
    }

    return SKILL_STAND;
}

VecPosition NaoBehavior::getRolePosition(Role role) {
    VecPosition rolePosition; // 用于存储角色位置

    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i) {
        WorldObject* player = worldModel->getWorldObject(i);
        if (player && player->validPosition) {
            Role assignedRole = agentRoles[i]; // 获取该球员的分配角色

            // 如果球员的分配角色与传入的角色相匹配
            if (assignedRole == role) {
                rolePosition = player->pos; // 获取该角色的位置
                break; // 找到对应角色后退出循环
            }
        }
    }

    return rolePosition; // 返回角色的位置
}

int NaoBehavior::getPlayerNumberForRole(Role role) {
    int playerNumber = -1; // 如果找不到匹配角色的球员，则返回 -1

    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i) {
        WorldObject* player = worldModel->getWorldObject(i);
        if (player && player->validPosition) {
            Role assignedRole = agentRoles[i]; // 获取该球员的分配角色

            // 如果球员的分配角色与传入的角色相匹配
            if (assignedRole == role) {
                playerNumber = i - WO_TEAMMATE1 + 1; // 获取球员编号（例如，WO_TEAMMATE1 对应 1 号球员）
                break; // 找到对应角色后退出循环
            }
        }
    }

    return playerNumber; // 返回球员编号
}


double NaoBehavior::distanceBetweenPoints(VecPosition p1, VecPosition p2)
{
    double x1 = p1.getX();
    double y1 = p1.getY();
    double x2 = p2.getX();
    double y2 = p2.getY();

    double distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return distance;
}

bool NaoBehavior::isOurTeammateWithBall()//有球权true
{
    VecPosition ballPos = worldModel->getBall();
    VecPosition my_postion=worldModel->getMyPosition();
   if((distanceBetweenPoints(ballPos, my_postion)<=0.3&&ballPos.getX()>=my_postion.getX())||(who_close_t.our_closest_distance>=who_close_t.opp_player_distance))
   {
    return true;
   } 
   else
   {
      return false;
   }
}




void NaoBehavior::who_close_to_ball(){
    int OurplayerClosestToBall = -1;
    int OppplayerClosestToBall = -1;
    double OurclosestDistanceToBall = 10000;
    double OppclosestDistancetoBall = 10000;
    VecPosition posball = worldModel->getBall();

    for (int i=WO_TEAMMATE1; i<WO_TEAMMATE1+NUM_AGENTS; ++i) {//WO_TEAMMATE1=1
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum()==playerNum) {
            temp = worldModel->getMyPosition();
        } else {
            WorldObject* teammate = worldModel->getWorldObject(i);
            if (teammate->validPosition) {
                temp = teammate->pos;
            } else {
                continue;
            }
        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);
        if (distanceToBall<OurclosestDistanceToBall) {
            if(!checkingFall()) {    
	      //自注：这里checkingfall的话，可能会存在防守被弹开问题。好处是在前锋的策略里可以避免把摔倒的人看做执行者
	      
                OurplayerClosestToBall = playerNum;
                OurclosestDistanceToBall = distanceToBall;
            }
        }
        who_close_t.our_closest_distance=OurclosestDistanceToBall;
        who_close_t.our_closest_player=OurplayerClosestToBall;
    }

    for (int j=WO_OPPONENT1; j<WO_OPPONENT1+NUM_AGENTS; ++j) {
        VecPosition temp;
        int OplayerNUm = j - WO_OPPONENT1 + 1;
        WorldObject* opponent = worldModel->getWorldObject(j);
        if(opponent->validPosition) {
            temp = opponent->pos;
        } else {
            continue;
        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);

        if (distanceToBall<OppclosestDistancetoBall)
         {
            OppplayerClosestToBall = OplayerNUm;
            OppclosestDistancetoBall = distanceToBall;
        }
        who_close_t.opp_player_distance=OppclosestDistancetoBall;
        who_close_t.opp_player_player=OppplayerClosestToBall;
    }
}

// 该函数实现决定是否进入传球模式

void NaoBehavior::ifpassmode() {
    // 获取球的位置
    VecPosition ballPosition = worldModel->getBall();

    // 获取我方最近的球员信息
    int ourClosestPlayer = who_close_t.our_closest_player;
    double ourClosestDistance = who_close_t.our_closest_distance;
     double pass_safe_distance=4;//离这个距离是不是进入危险区域
    // 获取对方最近的球员信息
    int oppClosestPlayer = who_close_t.opp_player_player;
    double oppClosestDistance = who_close_t.opp_player_distance;
   //还得判断敌方球员的位置
    //如果我方最近球员与对方最近球员的距离小于某个阈值，并且球的位置满足某些条件，则认为进入传球模式
    if (calculateOpponentDensity(pass_safe_distance)<0.5&&!getinpassmode) {
            // std::cout <<  "传球模式开启" << std::endl;
       
        getinpassmode = true;

    }
     else
     {
        //  std::cout <<  "传球模式关闭" << std::endl;
        getinpassmode = false;
    }
}
//判断球附近半径的敌方球员的密集程度

double NaoBehavior::calculateOpponentDensity(double searchRadius) {
    int opponentCount = 0;

    for (int j = WO_OPPONENT1; j < WO_OPPONENT1 + NUM_AGENTS; ++j) {
        VecPosition temp;
        WorldObject* opponent = worldModel->getWorldObject(j);

        if (opponent->validPosition) {
            temp = opponent->pos;
        } else {
            continue;
        }

        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);

        if (distanceToBall < searchRadius) {
            opponentCount++;
        }
    }

    // 计算疏密程度
    print3(opponentCount);
    double density = static_cast<double>(opponentCount) / searchRadius;
    //   std::cout <<  density << std::endl;
    return density;
}
//A号大区域的敌方疏密判断用于补球哥
int NaoBehavior::calculateZoneDensities_A() 
{ 
   std::vector<double> zoneScores(4, 1.0);

    // 循环遍历敌方球员（除守门员外）
    for (int j = WO_OPPONENT1; j < WO_OPPONENT1 + NUM_AGENTS; ++j) {
        WorldObject* opponent = worldModel->getWorldObject(j);

        if (opponent->validPosition) {
            VecPosition temp = opponent->pos;
            temp.setZ(0);

            // 判断球员位置在哪个区域
            if (temp.getY() > 0&& temp.getX() >=9 && temp.getX() <= 15 && temp.getY() <= 7) {
                zoneScores[0]++;
            } else if (temp.getY() <= 7 && temp.getX() >= 12 && temp.getX() < 15 && temp.getY() >= 0) {
                zoneScores[1]++;
            } else if (temp.getY() <= 0 && temp.getX() >= 9 && temp.getX() <= 12 && temp.getY() >= -7) {
                zoneScores[2]++;
            } else if (temp.getY() <=0 && temp.getX() >= 12 && temp.getX() < 15 && temp.getY() >=-7) {
                zoneScores[3]++;
            }
        }
    }

  // 找到最小的评分
    int minDensityIndex = 0;
    for (int i = 1; i < 4; ++i) {
        if (zoneScores[i] < zoneScores[minDensityIndex]) {
            minDensityIndex = i;
        }
    }

    return minDensityIndex + 1; // 返回区域编号（从1开始）
}
//左上区域对应1，右上区域对应2，左下区域对应3，右下区域对应4



int NaoBehavior::calculateZoneDensities() {
    // 定义四个区域的评分
    std::vector<double> zoneScores(4, 1.0);

    // 循环遍历敌方球员（除守门员外）
    for (int j = WO_OPPONENT1; j < WO_OPPONENT1 + NUM_AGENTS; ++j) {
        WorldObject* opponent = worldModel->getWorldObject(j);

        if (opponent->validPosition) {
            VecPosition temp = opponent->pos;
            temp.setZ(0);

            // 判断球员位置在哪个区域
            if (temp.getY() > 0 && temp.getX() > 0 && temp.getX() <= 7.5 && temp.getY() <= 10) {
                zoneScores[0]++;
            } else if (temp.getY() <= 0 && temp.getX() >= 7.5 && temp.getX() < 15 && temp.getY() >= -10) {
                zoneScores[1]++;
            } else if (temp.getY() <= 0 && temp.getX() > 0 && temp.getX() <= 7.5 && temp.getY() >= -10) {
                zoneScores[2]++;
            } else if (temp.getY() > 0 && temp.getX() >= 7.5 && temp.getX() <= 15 && temp.getY() <= 10) {
                zoneScores[3]++;
            }
        }
    }

  // 找到最小的评分
    int minDensityIndex = 0;
    for (int i = 1; i < 4; ++i) {
        if (zoneScores[i] < zoneScores[minDensityIndex]) {
            minDensityIndex = i;
        }
    }

    return minDensityIndex + 1; // 返回区域编号（从1开始）

}
//左上区域对应1，右上区域对应2，左下区域对应3，右下区域对应4

SkillType NaoBehavior::cy_findball(Role role)
{
        VecPosition posball = worldModel->getBall();
        VecPosition target_ball=VecPosition(posball.getX(), 0, 0);
        target_ball = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target_ball, false /*fKeepDistance*/);
        int number=worldModel->getUNum(); 
        if(posball.getX()>getRolePosition(role).getX()&&posball.getY()>getRolePosition(role).getY())
        {
            
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.8, posball.getY()-0.5, 0));
		             target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);

        }
        if(posball.getX()<getRolePosition(role).getX()&&posball.getY()>getRolePosition(role).getY())
        {
          
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.4, posball.getY()-0.7, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);
            
        }
        if(posball.getX()<getRolePosition(role).getX()&&posball.getY()<getRolePosition(role).getY())
        {
                VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.4, posball.getY()+0.7, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);
          
        }
        if(posball.getX()>getRolePosition(role).getX()&&posball.getY()<getRolePosition(role).getY())
        {
            
                    VecPosition target5 = VecPosition(VecPosition(posball.getX()-0.8, posball.getY()+0.5, 0));
		            target5 = collisionAvoidance(true/*Avoid teamate*/,false/*Avoid opponent*/,false/*Avoid ball*/, .5, 1, target5, false /*fKeepDistance*/);
                    return goToTarget(target5);
      
            
        }
} 


const double someTimeThreshold = 1.0; // 假设1秒为阈值

bool NaoBehavior::isBallMovingQuickly() {
    static VecPosition lastBall;
    static double lastTime = worldModel->getTime();

    VecPosition thisBall = worldModel->getBall();
    double currentTime = worldModel->getTime();

    double distanceCovered = distanceBetweenPoints(thisBall, lastBall);
    double timeElapsed = currentTime - lastTime;
    double ballSpeed = distanceCovered / timeElapsed; // 计算球的速度
    

    // 更新上一次球的位置和时间
    lastBall = thisBall;
    lastTime = currentTime;

    // 打印信息
    // std::cout << "上一次球的位置: " << lastBall << ", 这一次球的位置: " << thisBall << std::endl;
    // std::cout << "球移动的距离: " << distanceCovered << ", 时间差: " << timeElapsed << ", 球的速度: " << ballSpeed << std::endl;

    // 设置速度阈值，判断是否为大脚射门
    const double speedThreshold = 6; // 速度阈值
    const double distanceThreshold = 0.4; // 距离阈值
    const double timeThreshold =0.04; // 时间阈值
// && distanceCovered >= distanceThreshold && timeElapsed < timeThreshold  thisBall.getX() <lastBall.getX() &&
    // 确认球朝向我方球门且满足速度和距离条件
    if ( ballSpeed > speedThreshold && distanceCovered >= distanceThreshold && timeElapsed < timeThreshold) {
        // std::cout << "是大脚射门" << std::endl;
        return true;
    }

    // std::cout << "不是大脚射门" << std::endl;
    return false;
}

bool NaoBehavior::isBallWithinCriticalDistance() {
    // 定义扑救阈值
    const double CRITICAL_DISTANCE = 5.0;  // 例如5米

    // 获取球的位置
    VecPosition ballPos = worldModel->getBall();

    // 获取守门员的位置
    VecPosition goaliePos = worldModel->getMyPosition();

    // 计算球和守门员之间的距离
    double distanceToBall = ballPos.getDistanceTo(goaliePos);

    // 如果距离小于或等于阈值，则返回true
    return distanceToBall <= CRITICAL_DISTANCE;
}


bool NaoBehavior::isBallMovingTowardsOurGoal() {
    static VecPosition lastBall;
    const double angleTolerance = 3.0;
    VecPosition thisBall = worldModel->getBall();

    // 球门的位置
    VecPosition ourGoal(-15, 0, 0);

    // 计算球的运动方向角度和朝向球门的角度
    double ballMovementAngle = atan2Deg(thisBall.getY() - lastBall.getY(), thisBall.getX() - lastBall.getX());
    double ballToGoalAngle = atan2Deg(ourGoal.getY() - thisBall.getY(), ourGoal.getX() - thisBall.getX());

    // 打印球的运动方向角度和朝向球门的角度
    // std::cout << "球的运动方向角度: " << ballMovementAngle << ", 朝向球门的角度: " << ballToGoalAngle << std::endl;

    // 判断球是否朝向球门
    bool isMovingTowardsGoal = fabs(SIM::normalizeAngle(ballMovementAngle - ballToGoalAngle)) < angleTolerance &&
                               thisBall.getX() < lastBall.getX();

    // 更新上一次球的位置
    lastBall = thisBall;
  if(thisBall.getX()<1)  //粗略判断是不是在传球中还是射门
  {
    if (isMovingTowardsGoal) {
        // std::cout << "敌方射门" << std::endl;
        return true;
    }
    else
    {
        return false;
    }
  }
  else
    return false;
}

bool NaoBehavior::ifAnyplayerBehindBall(int thisplayernumber)//是否有除本球员以外的其他球员在球后方1以内的位置
{
    
    VecPosition ballPos = worldModel->getBall();
    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i) 
    {
        WorldObject* player = worldModel->getWorldObject(i);
        if (player && player->validPosition) 
        {
            VecPosition playerPos = player->pos;

            if (playerPos.getX() < ballPos.getX()-0.3&&playerPos.getY()< ballPos.getY()+0.5&&playerPos.getY()> ballPos.getY()-0.5&&me.getDistanceTo(ballPos)<1&&i!=thisplayernumber)
            {
                return true; // 有球员在球后方1以内的位置
            }
        }
    }
    return false;// 没有球员在球后方1以内的位置
}
// (ballPos.getX()-playerPos.getX())

void NaoBehavior::print1(string i)
{
    static int k; 
    k++;
    int printnumber=worldModel->getUNum();
    if(k==50)
    {
         std::cout <<printnumber << i << std::endl;
         k=0;
    }
}
void NaoBehavior::print2()
{
    static int g; 
    g++;
    // int printnumber=worldModel->getUNum();
    if(g==50)
    {
          cout << "这次新的角色分配结果"<<  role_ass_num_final<< endl;
         g=0;
    }
}

void NaoBehavior::print3(int a)
{
    static int g; 
    g++;
    // int printnumber=worldModel->getUNum();
    if(g==10)
    {
         std::cout << a << std::endl;
         g=0;
    }
}