/************************************************************************************************************
*   File Name   : 12-Path_decision.cpp
*   Discription : Path direction decision (Left/Center/Right) Robot should go Left, straight, or right.
*                 vision + decision
*                 # Human Thinking vs Robot thinking
*                       Human: "The path is more open on right, I'll turn right."
*                       Robot: "There are more WHITE pixel on the right."
*                   So we convert intuition -> Numbers
*                 # Divide the ROI into 3 Zones
*                       | LEFT | CENTER | RIGHT |
*                       Each zone:  Count WHITE pixels
*                                   More White = More free space
*   Author      : Omkar Ankush Kashid
*   Created On  : 06-01-2026
*   Last updated: 06-01-2026
*   Platform    : NVIDIA Jetson Nano Super 8GB
*   OS          : C++   

************************************************************************************************************/