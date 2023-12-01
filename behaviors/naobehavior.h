#ifndef NAOBEHAVIOR_H
#define NAOBEHAVIOR_H

#include "behavior.h"
#include "../headers/headers.h"
#include "../parser/parser.h"
#include "../worldmodel/worldmodel.h"
#include "../bodymodel/bodymodel.h"
#include "../particlefilter/PFLocalization.h"
#include "../skills/skill.h"

#include "skl.h" 

// For UT Walk
#include <MotionCore.h>
#include <memory/Memory.h>
#include <memory/FrameInfoBlock.h>
#include <memory/SensorBlock.h>
#include <memory/JointBlock.h>
#include <memory/JointCommandBlock.h>
#include <memory/SimEffectorBlock.h>
#include <memory/WalkRequestBlock.h>

#include "./params.h"

using namespace std;


// TODO: Temporary home. Not sure if this this the best place to put this.
struct WalkVelocity
{
    WalkRequestBlock::ParamSet paramSet;

    double x;   // X direction velocity (unspecified unit)
    double y;   // Y direction velocity
    double rot;   // Rotational velocity about the Z-axis

    WalkVelocity() : paramSet(WalkRequestBlock::PARAMS_DEFAULT), x(0), y(0), rot(0) {}

    WalkVelocity(const double& velX, const double& velY, const double& velRot) :
        paramSet(WalkRequestBlock::PARAMS_DEFAULT), x(velX), y(velY), rot(velRot) {}

    WalkVelocity(WalkRequestBlock::ParamSet param, const double& velX, const double& velY, const double& velRot) :
        paramSet(param), x(velX), y(velY), rot(velRot) {}

    friend std::ostream& operator<<(std::ostream &out, const WalkVelocity& v)
    {
        out << "Parameter Set: " << v.paramSet << " T: (" << v.x << ", " << v.y << ") |R: " << v.rot;
        return out;
    }
};
struct who_close{
    float our_closest_player;
    float our_closest_distance;
    float opp_player_player;
    float opp_player_distance;
};

class NaoBehavior : public Behavior {
    friend class KickClassifier;
protected:
SkillType HELPER_Tactic();
VecPosition football_box_2=VecPosition(15,0,0);
SkillType attack_time();
void printRole(unsigned long r);
string roleToString(Role role);
SkillType defend();
SkillType GOALIE_Tactic();
SkillType WING_RIGHT_Tactic();
SkillType cy_findball(Role role);
SkillType BACK_RIGHT_Tactic();
SkillType FORWARD_MID_Tactic();
bool isBallMovingTowardsOurGoal() ;
bool isBallWithinCriticalDistance();
bool isBallMovingQuickly() ;
SkillType BACK_MID_Tactic();
SkillType MID_LEFT_Tactic();
SkillType MID_RIGHT_Tactic();
void updateSatisfaction() ;
SkillType BACK_LEFT_Tactic() ;
SkillType pass()  ;
SkillType start_ball();
bool ifAnyplayerBehindBall(int thisplayernumber);
int calculateZoneDensities() ;
double isAnyPlayerAheadOfBall() ;
void print1(string i);
int calculateZoneDensities_A() ;
void print2();
void print3(int a);
bool isOurTeammateWithBall();
bool  islongshot();
SkillType SUPPORTER_Tactic();
SkillType FORWARD_LEFT_Tactic();
SkillType MID_MID_Tactic() ;
SkillType FORWARD_RIGHT_Tactic();
int getPlayerNumberForRole(Role role) ;
double distanceBetweenPoints(VecPosition p1, VecPosition p2);
    double currentFallStateStartTime;

    // TODO: eliminate these and use a better solution
    string classname;

    map< SkillType, boost::shared_ptr<Skill> > skills;
    const map<string, string>& namedParams;
    string rsg;

    std::string agentTeamName;
    int agentUNum;

    Parser *parser;
    WorldModel *worldModel;
    BodyModel *bodyModel;
    PFLocalization* particleFilter;

    // For UT Walk
    MotionCore* core;
    Memory *memory_;
    FrameInfoBlock* frame_info_;
    FrameInfoBlock* vision_frame_info_;
    SensorBlock* raw_sensors_;
    JointBlock* raw_joint_angles_;
    JointCommandBlock* raw_joint_commands_;
    JointBlock* processed_joint_angles_;
    JointCommandBlock* processed_joint_commands_;
    SimEffectorBlock* sim_effectors_;
    WalkVelocity velocity;
VecPosition getRolePosition(Role role) ;
    // For UT Walk
    void calculateAngles();
    void preProcessJoints();
    void postProcessJoints();

    double hoverTime;
    bool mInit;
    bool initBeamed;
    double beamTime;

    bool initialized;

    SkillType skill;
    int skillState;

    int fallState;
    bool fallenLeft, fallenRight, fallenDown, fallenUp;
    double fallTimeStamp;
    double fallTimeWait;
    SkillType go_back();
    VecPosition kickdirection; // temporary name for me->opponent
    VecPosition kickDirection;
    int kickType;
    VecPosition kickTarget;

    double lastGetupRecoveryTime;

    SkillType currentKick;
    int currentKickType;

    VecPosition me;
    VecPosition myXDirection, myYDirection, myZDirection;

    VecPosition ball;

    //SCORE
    int scoreMe;
    int scoreOpp;

    string monMsg;

    bool fParsedVision;
    string composeAction();
      void ifpassmode();
    virtual void resetSkills();
    void resetScales();
    void refresh();
    void act();
  double calculateOpponentDensity(double searchRadius);
   
    // ----------------------------------------------------
    // ---------  THESE FUNCTIONS ARE
    // ---------  TO BE OVERRIDEN BY AGENTS...
    virtual SkillType selectSkill();
    virtual void beam( double& beamX, double& beamY, double& beamAngle );
    virtual void updateFitness() {}

    // ----------------------------------------------------


    bool checkingFall();

    /**
     * Trims the value to within [min, max].
     *
     * value - The value to trim.
     * min   - The minimum that the value can be.
     * max   - The maximum that the value can be.
     */
    double trim(const double& value, const double& min, const double&max);

    /**
     * Returns the skill that will best approximate the desired
     * walk direction and rotation using the currently implemented
     * walk. This function was designed for use by selectSkill().
     * It allows for a more general implementation of selectSkill()
     * that does not depend (as much) on the currently implemented
     * walk. This function delivers the fastest possible speed, so it
     * is not appropriate for alignment/fine-tuning.
     *
     * For the purpose of this implementation, rotation = 0 is not
     * the same as 360 is not the same as -360. 0 means no rotation,
     * 360 means rotate to the left, while -360 means rotate to the
     * right.
     *
     * direction - The angle to walk in degrees relative to the
     *             direction the robot is facing.
     * rotation  - The angle in degrees to turn the robot.
     * speed     - The percentage of maximum walk speed to use. Should
     *             be a value between 0 and 1. Default 1. This argument
     *             does not affect turn speed.
     * fAllowOver180Turn - allow for turns greater than abs(180) instead
     *                     of mapping them to their complement
     *
     */
    SkillType getWalk(const double& direction, const double& rotation, double speed = 1.0, bool fAllowOver180Turn=false);
    SkillType getWalk(WalkRequestBlock::ParamSet paramSet, const double& direction, double rotation, double speed, bool fAllowOver180Turn=false);

    /**
     * Returns the skill that's needed to get to the target location facing the target
     * rotation. All targets are offsets relative to the robot's current location and
     * orientation. Note that something like (globalTarget - me) is NOT the correct local
     * target. g2l(globalTarget) should be used instead.
     */
    SkillType goToTargetRelative(const VecPosition& targetLoc, const double& targetRot, double speed=1, bool fAllowOver180Turn=false, WalkRequestBlock::ParamSet paramSet=WalkRequestBlock::PARAMS_DEFAULT);

    SkillType goToTarget(const VecPosition &target);

    VecPosition collisionAvoidance(bool avoidTeammate, bool avoidOpponent, bool avoidBall, double PROXIMITY_THRESH, double COLLISION_THRESH, VecPosition target, bool fKeepDistance=true);
    VecPosition collisionAvoidanceCorrection(VecPosition start, double PROXIMITY_THRESH, double COLLISION_THRESH, VecPosition target, VecPosition obstacle);
    VecPosition collisionAvoidanceApproach(double PROXIMITY_THRESH, double COLLISION_THRESH, VecPosition target, VecPosition obstacle);
    VecPosition collisionAvoidanceApproach(VecPosition start, double PROXIMITY_THRESH, double COLLISION_THRESH, VecPosition target, VecPosition obstacle);
    VecPosition navigateAroundBall(VecPosition target, double PROXIMITY_THRESH, double COLLISION_THRESH );

    void resetKickState();

    double computeKickCost(VecPosition target, SkillType kickType);
    SkillType kickBall(const int kickTypeToUse, const VecPosition &target);
    SkillType kickBallAtPresetTarget();

    void getTargetDistanceAndAngle(const VecPosition &target, double &distance, double &angle);

    SkillType kickBallAtTargetSimplePositioning( const VecPosition &targetToKickAt, SkillType kick_skill, int kickType);

    /**
     * Returns the maximum direction at which we can walk and still maintain maximum forward speed.
     * In other words, returns the angle theta such that if we walk in any direction in [-theta, theta],
     * our forward translation will be the maximum that the walk engine will allow.
     */
    double getLimitingAngleForward();

    bool beamablePlayMode();
    bool improperPlayMode();
    bool improperPlayMode(int pm);
    bool kickPlayMode();
    bool kickPlayMode(int pm, bool eitherTeam=false);
    bool isIndirectKick();
    bool isIndirectKick(int pm);
    void who_close_to_ball();
    void readSkillsFromFile( const std::string& filename);

    bool isRightSkill( SkillType skill );
    bool isLeftSkill( SkillType skill );

    double getParameter(const std::string& name);
    double getStdNameParameter(const SkillType kick_skill, const std::string& parameter);
    void getSkillsForKickType(int kickType, SkillType skillsForType[]);
    void mark_position_attack();
    void mark_position_defense();
    void init_dis();
    void attackordefense();

    SkillType SpecialMode();
    SkillType CF_Tactic();//踢球
    SkillType GK_Tactic();//守门员
    SkillType SP_Tactic();
    SkillType GK_Tactic_Pass();
    SkillType DMF_leftTactic();//后卫左
    SkillType DMF_rightTactic();//后卫右
    SkillType CORNER_Tactic();//特殊
    SkillType kickDribbleAvoid(const VecPosition &target);//无方向带球
    void mark_position();
    SkillType demoKickingCircle();
    SkillType final_attack();
    SkillType goalkeeper();
    SkillType judegment_playmode();
    map<int, Role> RoleAssign();
    int attacksignal = 1;
    VecPosition find_direction();
    VecPosition passPosition;
    int getPlayerClosestToBall();
    int getPlayerClosestToBall_1();
    SkillType goToSpace(VecPosition space);
        
    VecPosition getSpaceForRole(Role role);
    int getRoleNum(Role role);
    
    map<int, Role> getAgentRoles();
    
    double getAgentDistanceToRole(int uNum, Role role);
    
    int getOpponentClosestToBall();
    int getOpponentClosestToRole(VecPosition space);


    int chooseCF();
    bool dangeropp(VecPosition opp);
    
    unsigned long role_ass_co(unsigned long x);
    
    unsigned long role_ass_num_final;


    int on_position[20];
    int on_position_player[20];
    VecPosition Position[20];
    double G[50][50];
    
    VecPosition defense_Position[200] = {0};
    VecPosition attack_Position[200] = {0};
    VecPosition hereiam = 0, hereisball = 0;

    int playerClosestToBall_all = -1;
    int opponentClosestToBall_all = -1;
    double closestDistanceToBall_all = 10000;
    bool getinpassmode = 0;
    bool defendmode = 0;
    int passcount = 0;
    std::map<int, Role> agentRoles;
    
    void mdrawTargetPosition();
    
    void mdrawRoles();
    
    map<int, Role> KM_RoleAssign();
    
    unsigned long role_ass_num ;
    
    int best_info();
    
    void getval();
        
    SkillType shoot();
   

    SkillType pass_test();
    VecPosition GK_Tactic_pass();
    
    bool check_float();
        


public:

    NaoBehavior(const std::string teamName, int uNum, const map<string, string>& namedParams_, const string& rsg_);
    virtual ~NaoBehavior();

    virtual std::string Init();
    virtual std::string Think(const std::string& message);

    void setMonMessage(const std::string& msg);
    string getMonMessage();
    
    

    inline MotionCore* getCore() {
        return core;
    }
    
    void heard_mess(unsigned long x, int playernum);
};

#endif // NAOBEHAVIOR_H

