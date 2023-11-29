#include "naobehavior.h"


//#define MDRAW



void NaoBehavior::mdrawTargetPosition(){
#ifdef MDRAW
    const Role MyRole = agentRoles[worldModel->getUNum()];
   switch (MyRole) {
       case GOALIE:
           //            Text = generate_Debug_Text(true,true);
           //            worldModel->getRVSender()->drawText("Text", Text, -HALF_FIELD_X,
           //                                                FIELD_Y, RVSender::ORANGE);
           worldModel->getRVSender()->drawText("Role", "GK", getSpaceForRole(MyRole).getX(),
                                               getSpaceForRole(MyRole).getY(), RVSender::ORANGE);
           break;
       case ON_BALL:
           //            Text = generate_Debug_Text(true,true);
           //            worldModel->getRVSender()->drawText("Text", Text, -HALF_FIELD_X,
           //                                                FIELD_Y-0.5, RVSender::ORANGE);
           worldModel->getRVSender()->drawText("Role", "OB", getSpaceForRole(MyRole).getX(),
                                               getSpaceForRole(MyRole).getY(), RVSender::ORANGE);
           break;
       case SUPPORTER:
           //            Text = generate_Debug_Text(true,true);
           //            worldModel->getRVSender()->drawText("Text", Text, -HALF_FIELD_X,
           //                                                FIELD_Y-1, RVSender::ORANGE);
           worldModel->getRVSender()->drawText("Role", "SPT", getSpaceForRole(MyRole).getX(),
                                               getSpaceForRole(MyRole).getY(), RVSender::ORANGE);
           break;
       case BACK_LEFT:
           //            Text = generate_Debug_Text(true,true);
           //            worldModel->getRVSender()->drawText("Text", Text, -HALF_FIELD_X,
           //                                                FIELD_Y-1.5, RVSender::ORANGE);
           worldModel->getRVSender()->drawText("Role", "BL", getSpaceForRole(MyRole).getX(),
                                               getSpaceForRole(MyRole).getY(), RVSender::ORANGE);
           break;
       case BACK_RIGHT:
           //            Text = generate_Debug_Text(true,true);
           //            worldModel->getRVSender()->drawText("Text", Text, -HALF_FIELD_X,
           //                                                FIELD_Y-2, RVSender::ORANGE);
           worldModel->getRVSender()->drawText("Role", "BR", getSpaceForRole(MyRole).getX(),
                                               getSpaceForRole(MyRole).getY(), RVSender::ORANGE);
           break;
       case MID_LEFT:
           //            Text = generate_Debug_Text(true,true);
           //            worldModel->getRVSender()->drawText("Text", Text, -HALF_FIELD_X,
           //                                                FIELD_Y-2.5, RVSender::ORANGE);
           worldModel->getRVSender()->drawText("Role", "ML", getSpaceForRole(MyRole).getX(),
                                               getSpaceForRole(MyRole).getY(), RVSender::ORANGE);
           break;
       case MID_RIGHT:
           //            Text = generate_Debug_Text(true,true);
           //            worldModel->getRVSender()->drawText("Text", Text, -HALF_FIELD_X,
           //                                                FIELD_Y-3, RVSender::ORANGE);
           worldModel->getRVSender()->drawText("Role", "MR", getSpaceForRole(MyRole).getX(),
                                               getSpaceForRole(MyRole).getY(), RVSender::ORANGE);
           break;
       case WING_LEFT:
           //            Text = generate_Debug_Text(true,true);
           //            worldModel->getRVSender()->drawText("Text", Text, -HALF_FIELD_X,
           //                                                FIELD_Y-3.5, RVSender::ORANGE);
           worldModel->getRVSender()->drawText("Role", "WL", getSpaceForRole(MyRole).getX(),
                                               getSpaceForRole(MyRole).getY(), RVSender::ORANGE);
           break;
       case WING_RIGHT:
           //            Text = generate_Debug_Text(true,true);
           //            worldModel->getRVSender()->drawText("Text", Text, -HALF_FIELD_X,
           //                                                FIELD_Y-4, RVSender::ORANGE);
           worldModel->getRVSender()->drawText("Role", "WR", getSpaceForRole(MyRole).getX(),
                                               getSpaceForRole(MyRole).getY(), RVSender::ORANGE);
           break;
       case FORWARD_LEFT:
           //            Text = generate_Debug_Text(true,true);
           //            worldModel->getRVSender()->drawText("Text", Text, -HALF_FIELD_X,
           //                                                FIELD_Y-4.5, RVSender::ORANGE);
           worldModel->getRVSender()->drawText("Role", "FL", getSpaceForRole(MyRole).getX(),
                                               getSpaceForRole(MyRole).getY(), RVSender::ORANGE);
           break;
       case FORWARD_RIGHT:
           //            Text = generate_Debug_Text(true,true);
           //            worldModel->getRVSender()->drawText("Text", Text, -HALF_FIELD_X,
           //                                                FIELD_Y-5, RVSender::ORANGE);
           worldModel->getRVSender()->drawText("Role", "FR", getSpaceForRole(MyRole).getX(),
                                               getSpaceForRole(MyRole).getY(), RVSender::ORANGE);
           break;
   }
#endif
}




void NaoBehavior::mdrawRoles(){
#ifdef MDRAW
    const Role MyRole = agentRoles[worldModel->getUNum()];
   switch (MyRole) {
       case GOALIE:
           worldModel->getRVSender()->drawAgentText("GK", RVSender::RED);
           break;
       case ON_BALL:
           worldModel->getRVSender()->drawAgentText("OB", RVSender::RED);
           break;
       case SUPPORTER:
           worldModel->getRVSender()->drawAgentText("SPT", RVSender::RED);
           break;
       case BACK_LEFT:
           worldModel->getRVSender()->drawAgentText("BL", RVSender::RED);
           break;
       case BACK_RIGHT:
           worldModel->getRVSender()->drawAgentText("BR", RVSender::RED);
           break;
       case MID_LEFT:
           worldModel->getRVSender()->drawAgentText("ML", RVSender::RED);
           break;
       case MID_RIGHT:
           worldModel->getRVSender()->drawAgentText("MR", RVSender::RED);
           break;
       case WING_LEFT:
           worldModel->getRVSender()->drawAgentText("WL", RVSender::RED);
           break;
       case WING_RIGHT:
           worldModel->getRVSender()->drawAgentText("WR", RVSender::RED);
           break;
       case FORWARD_LEFT:
           worldModel->getRVSender()->drawAgentText("FL", RVSender::RED);
           break;
       case FORWARD_RIGHT:
           worldModel->getRVSender()->drawAgentText("FR", RVSender::RED);
           break;
       default:
           cerr << "UnKnown Role!";
   }
#endif
}
