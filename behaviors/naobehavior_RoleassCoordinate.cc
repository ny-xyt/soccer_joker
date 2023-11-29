#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"
#include<algorithm>
#include <sstream>
#include<bits/stdc++.h>


static unsigned long pre_deal_info[100];

static int val[15][5] = {0};

void NaoBehavior::heard_mess(unsigned long role_ass_info, int playernum) {
    //cout << role_ass_info << " " << playernum << endl;
    if (role_ass_info != 0) pre_deal_info[playernum] = role_ass_info;
    else {
        for (int j = 1; j <= 3; j++) {
            val[playernum][j] = 0;
        }
        pre_deal_info[playernum] = 0;
    }
}


int NaoBehavior::best_info() {

//     int check_box = 0;
//     for(int i = 1; i <= 11; i ++){
//         for(int j = 1; j <= 3; j ++)
//             check_box += val[i][j];
//     }
//     if(check_box == 0) {
//         for(int i = 1; i <= 11; i ++){
//             for(int j = 1; j <= 3; j ++)
//                 val[i][j] = 1;
//         }        
//     }

    int sum_val[100];
    for (int i = 0; i <= 50; i++)
        sum_val[i] = 0;
    for (int i = 1; i <= 11; i++) {
        if (worldModel->getFallenTeammate(i) && worldModel->getUNum() != i) {
            pre_deal_info[i] = 0;
            for (int j = 1; j <= 3; j++) {
                val[i][j] = 0;
            }
        }
        if (pre_deal_info[i] == 0) {
            for (int j = 1; j <= 3; j++) {
                val[i][j] = 0;
            }
        }
        for (int j = 1; j <= 3; j++) {
            sum_val[i] += val[i][j];
        }
    }

    bool hash_ass[15];
    for (int i = 1; i <= 11; i++) {
        hash_ass[i] = false;
    }
    int maxn = -1, max_pos = -1;
    for (int i = 1; i <= 11; i++) {
        if (hash_ass[i] == 1) continue;
        for (int j = i + 1; j <= 11; j++) {
            if (pre_deal_info[i] == pre_deal_info[j] && pre_deal_info[i] != 0) {
                hash_ass[j] = true;
                sum_val[i] += sum_val[j];
            }
        }
    }

    for (int i = 1; i <= 11; i++) {
        if (sum_val[i] > maxn) {
            maxn = sum_val[i];
            max_pos = i;
        }
    }
    return max_pos;

}

unsigned long NaoBehavior::role_ass_co(unsigned long role_ass_num) {
    //cout << "RA " <<  role_ass_num << " ";
    if (role_ass_num != 0) pre_deal_info[worldModel->getUNum()] = role_ass_num;
    int x = best_info();
    unsigned long long ans = x;
    for (int i = 1; i <= 11; i++) {
        for (int j = 3; j >= 2; j--) {
            val[i][j] = val[i][j - 1];
        }
    }
    for (int i = 1; i <= 11; i++) {
        if (ans == pre_deal_info[x]) {
            val[i][1] = 1;
        } else {
            val[i][1] = 0;
        }
    }
//     for(int i = 0; i <= 11; i ++)
//         cout << pre_deal_info[i] << endl;
//     cout << "NEST CYCLE" << endl;
    static unsigned long last_info = 0;
    if (pre_deal_info[x] != 0) last_info = pre_deal_info[x];
    return last_info;
}


// void NaoBehavior::shouldEnterPassMode()