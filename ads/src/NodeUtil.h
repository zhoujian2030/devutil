/*
 * NodeUtil.h
 *
 *  Created on: May 30, 2016
 *      Author: z.j
 */
 
#ifndef NODE_UTIL_H
#define NODE_UTIL_H

namespace ads {
    
    class Node {
    public:
        int val;
        Node* next;
        Node(int val=0):val(val),next(0){}
    };
    
    class NodeUtil {
    public:     
        // @description - check if there is loop exists in a node list
        // @param head - head of the node list
        // @return - true if loop exists   
        static bool findLoop(Node* head);
        
        // @description - if there is loop existed like below, return N6
        //      as the tail node.
        //      N1 --> N2 --> N3 --> N4
        //                    |      |
        //                    N6 <-- N5
        // @param head - head of the node list
        // @return - 0 if node list is empty
        static Node* findTail(Node* head);
        
        // @description - generate a single node list according array
        static Node* generateNonLoopNodeList(int* array, int length);  
        
        // @description - create a new node and append it to the tail,
        //      don't call this method if the list has loop
        // @param tail - tail of the node list, if not the tail, will 
        //               find the tail first
        // @param val - value of the new node to be added in tail
        // @return tail of the list
        static Node* addNodeToTailByVal(Node* tail, int val);
        
        // @description - append a node to the tail, don't call this 
        //      method if the list has loop
        // @param tail - tail of the node list, if not the tail, will 
        //      find the tail first
        // @param newNode - new node to be added in tail
        // @return tail of the list        
        static Node* addNodeToTailByNode(Node* tail, Node* newNode);
               
        static void printNodeList(Node* head);
    };
    
}

#endif