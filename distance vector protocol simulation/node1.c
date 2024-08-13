#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "DV.h"
#include "node1.h"

#define NODE_ID 1


// Link costs to directly connected neighbours used for initializing the distance table.
int connectcosts1[4] = { 1, 0, 2, 3 };

// Minimum costs to all nodes, initially set to same as connectcosts
int mincosts1[4] = { 1, 0, 2, 3 };

struct distance_table dt1;

void findmincosts1() {
    // Update mincosts if distance table was updated.
    for (int i = 0; i < 4; i++) {
        // Ignore link cost to ourself
        if (i == NODE_ID) {
            continue;
        }

        for (int j = 0; j < 4; j++) {
            // Ignore routes to another node via ourself, which we already have from connectcosts[]
            if (j == NODE_ID) {
                continue;
            }

            // Ignore uninitialized link costs
            if (dt1.costs[i][j] == 0) {
                continue;
            }

            // Update mincost[i] with lowest cost for node i from each of the options of dt1.costs[i][j]
            if (dt1.costs[i][j] < mincosts1[i]) {
                mincosts1[i] = dt1.costs[i][j];
            }
        }
    }
}

void sendcosts1() {
    struct rtpkt cost_pkt;
    cost_pkt.sourceid = NODE_ID;
    cost_pkt.destid = 0;
    memcpy(&cost_pkt.mincost, mincosts1, 4 * sizeof(int));

    struct rtpkt *cost_pkt_ptr = &cost_pkt;

    for (int i = 0; i < 4; i++) {
        // ignore self and non directly connected nodes (999)
        if (i == NODE_ID || connectcosts1[i] == 999) {
            continue;
        }

        cost_pkt_ptr->destid = i;
        tolayer2(cost_pkt);
    }
}

/* students to write the following two routines, and maybe some others */
void rtinit1() {
    // Initialize distance table
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                dt1.costs[i][j] = connectcosts1[i];
            }
            else {
                dt1.costs[i][j] = 999;
            }
        }
    }

    sendcosts1();
    printdt1();
}


void rtupdate1(struct rtpkt *rcvdpkt) {
    printf("----------\n");
    printf("rtupdate1 srcid: %i\n", rcvdpkt->sourceid);
    printf("rtupdate1 destid: %i\n", rcvdpkt->destid);
    printf("rtupdate1 mincosts: ");
    for (int i = 0; i < 4; i++) {
        printf("%i ", rcvdpkt->mincost[i]);
    }
    printf("\n");

    // Steps to take:
    // - Check srcid (use as index for 2d distance table)
    // - Iterate through mincosts array (index is destination node)
    // - If connectcosts0[sourceid] + rcvdpkt->mincosts[i] < connectcosts0[sourceid] + dt1.costs[sourceid][i] then update dt1.costs[sourceid][i]
    // - If update made in last step, then resend dt1 to directly connected nodes

    bool table_updated = false;

    printf("----------\n");

    for (int i = 0; i < 4; i++) {
        // Don't update the distance table with another node's link cost to itself (which will always be 0)
        if (i == rcvdpkt->sourceid) {
            printf("i %i matches sourceid %i. Skipping.\n", i, rcvdpkt->sourceid);
            continue;
        }

        // Don't update the distance table with another node's link cost back to the current node (which we already have from mincosts[])
        if (i == NODE_ID) {
            printf("i %i matches NODE_ID %i. Skipping.\n", i, NODE_ID);
            continue;
        }

        // Ignore link costs of 999, which mean a node has not yet established a path to another node.
        if (rcvdpkt->mincost[i] == 999) {
            printf("node %i has not yet established a path to node %i. Skipping.\n", rcvdpkt->sourceid, i);
            continue;
        }

        printf("dt1.costs[i][srcid]: %i\n", dt1.costs[i][rcvdpkt->sourceid]);
        printf("connectcosts1[srcid]: %i\n", connectcosts1[rcvdpkt->sourceid]);
        printf("connectcost1[srcid] + rcvdpkt->mincost[i]: %i\n", connectcosts1[rcvdpkt->sourceid] + rcvdpkt->mincost[i]);
        if (dt1.costs[i][rcvdpkt->sourceid] == 0 || (connectcosts1[rcvdpkt->sourceid] + rcvdpkt->mincost[i]) < dt1.costs[i][rcvdpkt->sourceid]) {
            dt1.costs[i][rcvdpkt->sourceid] = connectcosts1[rcvdpkt->sourceid] + rcvdpkt->mincost[i];
            table_updated = true;
        }
    }

    printf("----------\n");

    if (table_updated == true) {
        printf("dt1 was updated. New table below.\n\n");
        printdt1();

        findmincosts1();

        printf("dt1update mincosts1: ");
        for (int i = 0; i < 4; i++) {
            printf("%i ", mincosts1[i]);
        }
        printf("\n");

        printf("dt1update sending out new min costs.\n");
        sendcosts1();
    }
}


void printdt1() {
    printf("------------------------\n");
    printf("                via     \n");
    printf("   D0 |    0     2    3 \n");
    printf("------|-----------------\n");
    printf("     0|  %3d   %3d   %3d\n", dt1.costs[0][0], dt1.costs[0][2], dt1.costs[0][3]);
    printf("dest 2|  %3d   %3d   %3d\n", dt1.costs[2][0], dt1.costs[2][2], dt1.costs[2][3]);
    printf("     3|  %3d   %3d   %3d\n", dt1.costs[3][0], dt1.costs[3][2], dt1.costs[3][3]);
    printf("------------------------\n");
}
