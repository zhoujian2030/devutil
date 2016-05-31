#include "NodeUtil.h"
#include <stdlib.h>
#include <sys/time.h>
#include <iostream>

using namespace std;
using namespace ads;

// --------------------------------------------
bool NodeUtil::findLoop(Node* head) {
    if (head == 0) {
        return false;
    }
    
    Node* slow = head;
    Node* fast = head;
    
    while (fast !=0 && fast->next != 0) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            return true;
        }
    }
    
    return false;
}

// ---------------------------------------------
Node* NodeUtil::findTail(Node* head) {
    if (head == 0) {
        return 0;
    }

    Node* slow = head;
    Node* fast = head;
    Node* tail = head;
    bool foundLoop = false;
    
    while (fast !=0 && fast->next != 0) {
        // find the tail in case no loop
        tail = fast->next;
        
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            foundLoop = true;
            break;
        }
    }    
    
    // no loop, return the tail
    if (!foundLoop) {
        if (fast != 0) {
            return fast;
        } else {
            return tail;
        }
    }
    
    // find the tail in loop
    Node* loopNode = slow;
    Node* tempNode = slow;
    while (true) {
        do {
            if (tempNode->next == head) {
                return tempNode;
            }
            tempNode = tempNode->next;
        } while (tempNode != loopNode);
        
        // tail not found, continue
        head = head->next;
    }
}

// ---------------------------------------------
Node* NodeUtil::addNodeToTailByVal(Node* tail, int val) {
    if (tail == 0) {
        tail = new Node(val);
    } else {
        // make sure we go to the tail
        while(tail->next != 0) {
            tail = tail->next;
        }
        tail->next = new Node(val);
        tail = tail->next;
    }
    
    return tail;
}

// ---------------------------------------------
Node* NodeUtil::addNodeToTailByNode(Node* tail, Node* newNode) {
    if (tail == 0) {
        tail = newNode;
    } else {
        // make sure we go to the tail
        while(tail->next != 0) {
            tail =  tail->next;
        }
        tail->next = newNode;
        tail = tail->next;
    }
    return tail;
}

// ---------------------------------------------
void NodeUtil::printNodeList(Node* head) {
    cout << "Show the node list: " << endl;
    cout << "+------------------------------------------------+" << endl;
    int i = 0;
    while (head != 0) {
        cout << head->val << "  ";
        head = head->next;
        i++;
        if (i != 0 && (i % 10 == 0)) {
            cout << endl;
        }
    }
    if (i%10 != 0) {
        cout << endl;
    }
    cout << "+------------------------------------------------+" << endl;
}

// ---------------------------------------------
Node* NodeUtil::generateNonLoopNodeList(int* array, int length) {
    if (length <= 0) {
        return 0;
    }
    
    Node* head = new Node(array[0]);    
    Node* tail = head;
    for (int i=1; i<length; ++i) {
        tail = NodeUtil::addNodeToTailByVal(tail, array[i]);
    }
    
    // NodeUtil::printNodeList(head);
    
    return head;
}

