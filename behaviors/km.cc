#include <iostream>
#include <cstring>
#include <cstdio>
#include "naobehavior.h"
 
using namespace std;
const int MAXN = 30;
 
int love[MAXN][MAXN];   // 记录每个妹子和每个男生的好感度  love[role][agent]
int ex_girl[MAXN];      // 每个妹子的期望值
int ex_boy[MAXN];       // 每个男生的期望值
bool vis_girl[MAXN];    // 记录每一轮匹配匹配过的女生
bool vis_boy[MAXN];     // 记录每一轮匹配匹配过的男生
int match[MAXN];        // 记录每个agent匹配到的role 如果没有则为-1  match[agent] = role
int slack[MAXN];        // 记录每个agent如果能被role倾心最少还需要多少期望值  slack[agent] = role
// man -> agent woman -> pos
const int INF_DIS = 1e9;
static int last_match[11];
 bool isSatisfied =true ;/* 根据你的逻辑判断是否满意 */
int N;

bool dfs(int girl)
{
    vis_girl[girl] = true;
 
    for (int boy = 0; boy < N; ++boy) {
 
        if (vis_boy[boy]) continue; // 每一轮匹配 每个男生只尝试一次
 
        int gap = ex_girl[girl] + ex_boy[boy] - love[girl][boy];
 
        if (gap  == 0) {  // 如果符合要求
            vis_boy[boy] = true;
            if (match[boy] == -1 || dfs( match[boy] )) {    // 找到一个没有匹配的男生 或者该男生的妹子可以找到其他人
                match[boy] = girl;
                return true;
            }
        } else {
            slack[boy] = min(slack[boy], gap);  // slack 可以理解为该男生要得到女生的倾心 还需多少期望值 取最小值 备胎的样子【捂脸
        }
    }
 
    return false;
}
 
double KM()
{
    memset(match, -1, sizeof match);    // 初始每个男生都没有匹配的女生
    memset(ex_boy, 0, sizeof ex_boy);   // 初始每个男生的期望值为0
    
    // 每个女生的初始期望值是与她相连的男生最大的好感度
    for (int i = 0; i < N; ++i) {
        ex_girl[i] = love[i][0];
        for (int j = 1; j < N; ++j) {
            ex_girl[i] = max(ex_girl[i], love[i][j]);
        }
    }
 
    
    // 尝试为每一个女生解决归宿问题
    for (int i = 0; i < N; ++i) {
        for(int j = 0; j <= 10; j ++)
            slack[j] = 100000000;
   
        while (1) {
            // 为每个女生解决归宿问题的方法是 ：如果找不到就降低期望值，直到找到为止
 
            // 记录每轮匹配中男生女生是否被尝试匹配过
            memset(vis_girl, false, sizeof(vis_girl));
            memset(vis_boy, false, sizeof(vis_boy));
 
            if (dfs(i)) break;  // 找到归宿 退出
 
            // 如果不能找到 就降低期望值
            // 最小可降低的期望值
            int d = 100000000;
            for (int j = 0; j < N; ++j)
                if (!vis_boy[j]) d = min(d, slack[j]);
 
            for (int j = 0; j < N; ++j) {
                // 所有访问过的女生降低期望值
                if (vis_girl[j]) ex_girl[j] -= d;
 
                // 所有访问过的男生增加期望值
                if (vis_boy[j]) ex_boy[j] += d;
                // 没有访问过的boy 因为girl们的期望值降低，距离得到女生倾心又进了一步！
                else slack[j] -= d;
            }
        }
    }
 
    // 匹配完成 求出所有配对的好感度的和
    double res = 0;
    for (int i = 0; i < N; ++i)
        res += love[ match[i] ][i];
 
    return res;
}
 
map<int, Role> NaoBehavior::KM_RoleAssign() //角色分配系统
{
    
     // 在角色分配之前保存当前的角色分配结果
    unsigned long lastRoleAssignment = role_ass_num_final;

    // 在角色分配之前保存当前的标志符状态
    bool lastSatisfactionStatus = isSatisfied;
    int FakeCF = 0;
    N = 9;
    agentRoles[1] = GOALIE;
    int the_CF = chooseCF();
    // if(the_CF < 2 || the_CF > 11){
    //     worldModel->getRVSender()->drawText("CF ERR", me.getX(), me.getY() + 0.3, RVSender::YELLOW);
    // }
    agentRoles[the_CF] = HELPER;
    for(int i = WO_TEAMMATE2; i < WO_TEAMMATE1 + 11; i ++){
        if(i - WO_TEAMMATE1 + 1 == the_CF) continue;
        VecPosition playerpos = 0;
        if(worldModel->getUNum() == i - WO_TEAMMATE1 + 1){
            playerpos = worldModel -> getMyPosition();
        }//如果当前球员的编号等于自己的编号，那么将 playerpos 设置为自己的位置。这是为了确保在计算与自己的“好感度”时使用自己的位置信息。
        else playerpos = worldModel->getTeammate(i);//将 playerpos 设置为当前球员的位置。
        for (int r = 1; r < NUM_ROLES - 1; r++) {
            Role role = static_cast<Role>(r);
            int pos = 0;
            if(i < the_CF) pos = i - WO_TEAMMATE2;
            else pos = i - WO_TEAMMATE2 - 1;
            love[r - 1][pos] = -getAgentDistanceToRole(i, role) * 100000;
        }
    }

//      cout << "1" << endl;
//      for(int i = 0; i < 9; i ++){
//          for(int j = 0; j < 9; j ++){
//              cout << love[i][j] << " "; 
//          }
//           cout << endl;
//      }
    double k = KM();


    for(int i = 0; i < 9; i ++)
        last_match[i] = match[i];
    
    for(int i = 0; i < 9; i ++){
        int playernum = 0;
        if(i + 2 < the_CF) playernum = i + 2;
        else playernum = i + 3;
        Role role = static_cast<Role>(match[i] + 1);
        agentRoles[playernum] = role;
    }

//     for(int i = 0; i < 9; i ++){   //role
//         Role role = static_cast<Role>(i + 1);
//         if(match[i] + 2 < the_CF) agentRoles[match[i] + 2] = role;
//         else agentRoles[match[i] + 3] = role;
//     }
    unsigned long role_ass_num = 0;
    for(int i = 2; i <= 11; i ++){ 
        Role r = agentRoles[i];
        int x = 0;
         if(r == FORWARD_MID) x = 0;
        if(r == FORWARD_LEFT) x = 1;
        if(r == FORWARD_RIGHT) x = 2;
        if(r == MID_LEFT) x = 3;
        if(r == MID_RIGHT) x = 4;
        if(r == MID_MID) x= 5;
        if(r == BACK_LEFT) x = 6;
        if(r == BACK_MID) x = 7;
        if(r == BACK_RIGHT) x = 8;
        if(r == HELPER) x = 9;   //不同角色对应的x
        role_ass_num = role_ass_num * 10 + x;  //其中每一位都代表一个特定的角色比如036，SUPPORTER，MID_LEFTWING_RIGHT)
    }
//     cout << role_ass_num << endl;
    role_ass_num_final = role_ass_num; 
    
    if(check_float()) //重新分配角色
    {
        role_ass_num_final = 0;
        role_ass_num = 0;
    } 
    
    unsigned long role_ass_num_final_cp = role_ass_co(role_ass_num);
    unsigned long restore_num = role_ass_num_final_cp;
    
    agentRoles[1] = GOALIE;
    
    int flag = 0;
    int flag_CF = 0;
    if(role_ass_num_final_cp < 1000000000) flag = 1;
     //检查是否满意，如果不满意且标志符为 false，则恢复上一次的角色分配结果
    if (!isSatisfied) {
          cout << "保留上次的角色分配结果"<<  lastRoleAssignment<< endl;
          printRole(lastRoleAssignment);
        role_ass_num_final = lastRoleAssignment;
        // 还可以进行其他处理，例如在控制台输出提示信息等   
      
    }
    else
    {
        printRole(lastRoleAssignment);
        // print2();
    }
    
    // printRole(role_ass_num_final_cp);
    for(int i = 11; i >= 2; i--){
        if(i == 2 && flag == 1) agentRoles[i] = BACK_MID;
        agentRoles[i] = static_cast<Role>(role_ass_num_final_cp % 10 + 1);  //角色分配
        if(role_ass_num_final_cp % 10 + 1 == 10){
            flag_CF = 1;
        }
        role_ass_num_final_cp =  role_ass_num_final_cp / 10;
    }//挨个提出数字来进行角色分配

    
    return agentRoles;
}   

void NaoBehavior::updateSatisfaction() {
    VecPosition posball=worldModel->getBall();
    // 在函数内部进行条件检查和更新 isSatisfied 的逻辑
    double a=distanceBetweenPoints(getRolePosition(BACK_LEFT),posball);
    double b=distanceBetweenPoints(getRolePosition(HELPER),posball);
    // if (a<=b) {
    //     // 根据你的条件设定 isSatisfied 为 true 或 false
    //     isSatisfied = false;
    // } 
    // else {
        isSatisfied = true;
    // }

    // // 可以在这里添加其他逻辑或输出信息
    // if (isSatisfied) {
    //     std::cout << "Satisfaction is true." << std::endl;
    // } else {
    //     std::cout << "Satisfaction is false." << std::endl;
    }


void NaoBehavior::printRole(unsigned long r)
{
    static int g; 
    g++;
   
    // 将数字转换为字符串
    string roleString = to_string(r);
     if(g==15)
    {
    // 输出每一位对应的球员和角色
    for (int i = 0; i < roleString.length(); ++i) {
        int playerNum = i + 2;  // 假设球员编号从2开始
        Role role = static_cast<Role>(roleString[i] - '0' + 1);  // 角色从1开始，所以需要加1

        // 输出结果
        cout <<playerNum << " " << roleToString(role) << endl;
         g=0;
        
    }
    // int printnumber=worldModel->getUNum();
    } 
}

string NaoBehavior::roleToString(Role role)
{
    switch (role) {
        case BACK_MID:
            return "BACK_MID";
        case BACK_LEFT:
            return "BACK_LEFT";
        case BACK_RIGHT:
            return "BACK_RIGHT";
        case MID_LEFT:
            return "MID_LEFT";
        case MID_RIGHT:
            return "MID_RIGHT";
        case MID_MID:
            return "MID_MID";
        case FORWARD_MID:
            return "FORWARD_MID";
        case FORWARD_LEFT:
            return "FORWARD_LEFT";
        case FORWARD_RIGHT:
            return "FORWARD_RIGHT";
        case HELPER:
            return "HELPER";
        case GOALIE:
            return "GOALIE";
        default:
            return "UNKNOWN_ROLE";
    }
}
