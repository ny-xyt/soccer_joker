#!/bin/bash

# 训练模式
sudo sed -i '/^$enableRealTimeMode/c$enableRealTimeMode = false' /usr/local/share/rcssserver3d/rcssserver3d.rb
sudo sed -i '/^$agentSyncMode/c$agentSyncMode = true' ~/.simspark/spark.rb
sudo sed -i '/[(]setSenseMyPos/c\            (setSenseMyPos true)' /usr/local/share/rcssserver3d/rsg/agent/nao/naoneckhead.rsg
sudo sed -i '/[(]setSenseMyOrien/c\            (setSenseMyOrien true)' /usr/local/share/rcssserver3d/rsg/agent/nao/naoneckhead.rsg
sudo sed -i '/[(]setSenseBallPos/c\            (setSenseBallPos true)' /usr/local/share/rcssserver3d/rsg/agent/nao/naoneckhead.rsg
sudo sed -i "/^addSoccerVar('BeamNoiseXY'/caddSoccerVar('BeamNoiseXY',0.0)" /usr/local/share/rcssserver3d/naosoccersim.rb #0.05
sudo sed -i "/^addSoccerVar('BeamNoiseAngle'/caddSoccerVar('BeamNoiseAngle',0.0)" /usr/local/share/rcssserver3d/naosoccersim.rb #10.0
sudo sed -i "/^addSoccerVar('RuleHalfTime'/caddSoccerVar('RuleHalfTime', 40.0 * 60)" /usr/local/share/rcssserver3d/naosoccersim.rb #addSoccerVar('RuleHalfTime', 40.0 * 60)
sudo sed -i "/^addSoccerVar('SelfCollisionBeamPenalty'/caddSoccerVar('SelfCollisionBeamPenalty', false)" /usr/local/share/rcssserver3d/naosoccersim.rb #false
sudo sed -i "/^addSoccerVar('SelfCollisionsTolerance'/caddSoccerVar('SelfCollisionsTolerance', 0.04)" /usr/local/share/rcssserver3d/naosoccersim.rb #0.04
sudo sed -i "/^addSoccerVar('SelfCollisionBeamCooldownTime'/caddSoccerVar('SelfCollisionBeamCooldownTime', 10.0)" /usr/local/share/rcssserver3d/naosoccersim.rb #10.0
