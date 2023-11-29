#include "pkbehaviors.h"

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
/////// GOALIE
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

PKGoalieBehavior::
PKGoalieBehavior( const std::string teamName,
                  int uNum,
                  const map<string, string>& namedParams_,
                  const string& rsg_)
    : NaoBehavior( teamName,
                   uNum,
                   namedParams_,
                   rsg_) {
}

void PKGoalieBehavior::
beam( double& beamX, double& beamY, double& beamAngle ) {
    beamX = -HALF_FIELD_X+.5;
    beamY = 0;
    beamAngle = 0;
}




SkillType PKGoalieBehavior::
selectSkill() {
    VecPosition localBall = worldModel->g2l(ball);
    SIM::AngDeg localCenterAngle = atan2Deg(localBall.getY(), localBall.getX());
    VecPosition target = VecPosition(-14.5,0,0); // Target of the goalie
    VecPosition localTarget = worldModel->g2l(target);
    double distanceToTarget = me.getDistanceTo(target);
    double distanceToBall = me.getDistanceTo(ball);
    if (localBall.getX() > 0) {return SKILL_STAND;}
    else{
        if (distanceToBall <= 4) {return localBall.getY() < 0?FALL_RIGHT:FALL_LEFT;}
        else{
           if(distanceToTarget < 0.25 && localCenterAngle < 10){
                // close enough to the target so just stand
                return SKILL_STAND;
            }
            if(distanceToTarget<0.5){
                // close enough to turn to the ball
                return goToTargetRelative(localTarget, localCenterAngle);
            }
            else{
                // not close to the target
                return goToTarget(target);
            }  
        }
       
    }
    
    // return SKILL_BLOCK_SIDE_LEFT;
    // return kickBall(KICK_SHORT, VecPosition(15,0,0));
    // return SKILL_STAND;
}





////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
/////// SHOOTER
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
PKShooterBehavior::
PKShooterBehavior( const std::string teamName,
                   int uNum,
                   const map<string, string>& namedParams_,
                   const string& rsg_ )
    : NaoBehavior( teamName,
                   uNum,
                   namedParams_,
                   rsg_ ) {
}

void PKShooterBehavior::
beam( double& beamX, double& beamY, double& beamAngle ) {
    beamX = 10;
    beamY = 0;
    beamAngle = 0;
}

SkillType PKShooterBehavior::
selectSkill() {
    VecPosition localBall = worldModel->g2l(ball);
    if (localBall.getX()<0){ return SKILL_STAND;}
    else return kickBall(KICK_LOW, VecPosition(15,0.8,0));
}
