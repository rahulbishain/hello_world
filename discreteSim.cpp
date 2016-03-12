#include "minMaxHeap.hpp"
#include <iostream>
#include <vector>
using namespace std;

class Microbe {
    public:
        int id;//CANT I AVOID IT HOW DO I COMPARE TWO MICROBES OTHERWISE?
        int initialStr;
        int currStr;
        int n;
        int infectedAt;

        Microbe() {
            id = -1;
            initialStr = -1;
            currStr = -1;
            n = -1;
            infectedAt = -1;
        }

        Microbe(int _id, int _initialStr, int _currStr, int _n, int _infectedAt) {
            id = _id;
            initialStr = _initialStr;
            currStr = _currStr;
            n = _n;
            infectedAt = _infectedAt;
        }

        void operator=(const Microbe &m){
            id = m.id;
            initialStr = m.initialStr;
            currStr = m.currStr;
            n = m.n;
            infectedAt = m.infectedAt;
        }

        bool operator<(const Microbe &m){
            return (id<m.id);
        }
};

class TimeEvent{
    public:
        int timeStep;
        Microbe* microbe;	//MAKE THE < OPERATOR FOR MICROBES and TIMEEVENT

        void operator=(const TimeEvent &m){	//NOT NEEDED
            timeStep = m.timeStep;
            microbe = m.microbe;
        }

        bool operator<(const TimeEvent &m){
            if (timeStep<m.timeStep)
                return true;
            else if (timeStep = m.timeStep)
                return (microbe->id<m.microbe->id);
            else 
                return false;
        }
};


int main(){//accept input
    int n;
    cin>>n;
    int maxMicrobeId = n-1; //assuming first microbe id = 0
    int lastMicrobeIndex = n-1;//to keep track of microbe id to be allotted at a new birth
    int currMicrobeCount = n-1;//current microbe count at t=0. used for next microbe id generation

    int strInp[n];
    for (int i=0;i<n;i++)
        cin>>strInp[i];

    int m;
    cin>>m;
    int infectionTimeInp[m][2];
    for (int i=0;i<m;i++){
        cin>>infectionTimeInp[i][0]>>infectionTimeInp[i][1];
        maxMicrobeId = (maxMicrobeId<infectionTimeInp[i][0])?infectionTimeInp[i][0]:maxMicrobeId;
    }
    int endTime;
    cin>>endTime;
    //accepting input over

    //start reading the input and create time events. Maintain them in a heap
    vector<Microbe> microbes(maxMicrobeId+1);
    for (int i=0;i<m;i++){
        int tempId = infectionTimeInp[i][0];
        int tempTime = infectionTimeInp[i][1];
        microbes[tempId].infectedAt = tempTime;
    }

    MinMaxHeap<TimeEvent> eventHeap;
    TimeEvent tempEvent;
    for (int i=0;i<n;i++){
        microbes[i].id = i;
        microbes[i].initialStr = strInp[i];
        microbes[i].currStr = strInp[i];
        microbes[i].n = (((i+strInp[i])*29)%3);
        tempEvent.microbe = &microbes[i];
        if (microbes[i].n>=microbes[i].infectedAt && microbes[i].infectedAt>=0){
            microbes[i].currStr = 0;
            tempEvent.timeStep =  microbes[i].infectedAt + (microbes[i].n==microbes[i].infectedAt?microbes[i].initialStr--:microbes[i].initialStr);
        }
        else
            tempEvent.timeStep = microbes[i].n;
        if (tempEvent.timeStep<=endTime){
            eventHeap.insert(tempEvent);
            cout<<"inserted in heap id "<<microbes[i].id<<endl;
            cout<<"inserted in heap strength "<<microbes[i].initialStr<<endl;
            cout<<"inserted in heap n "<<microbes[i].n<<endl;
            cout<<"inserted in heap infectedAt "<<microbes[i].infectedAt<<endl;
        }
    }

    cout<<"allotted "<<eventHeap.getHeapSize()<<endl;
    //Simulation
    while(eventHeap.getHeapSize()>0){
        TimeEvent currTime;//holds the current time being simulated 
        currTime=eventHeap.getMin();
        while(currTime.timeStep==eventHeap.getMin().timeStep && eventHeap.getHeapSize()>0 && currTime.timeStep<=endTime){
            cout<<"checking for curr time = "<<currTime.timeStep<<" endTime="<<endTime<<endl;
            if (currTime.microbe->currStr==0)
                cout<<"d "<<currTime.microbe->id<<" "<<currTime.timeStep<<endl;
            else{
                int tempId = ++lastMicrobeIndex;
                int tempInitialStr = currTime.microbe->initialStr;
                int tempCurrStr = tempInitialStr;
                int tempn = (((tempId + tempInitialStr)*29)%37);
                int tempInfectedAt = -1;
                int nextTimeStep = currTime.timeStep;
                cout<<"b "<<tempId<<" "<<currTime.timeStep<<endl;
                if (tempId==microbes.size()){
                    nextTimeStep+=tempn; 
                }
                else{
                    if (microbes[tempId].infectedAt<=nextTimeStep + tempn){
                        tempCurrStr = 0;
                        tempInfectedAt = microbes[tempId].infectedAt;
                        if (microbes[tempId].infectedAt==nextTimeStep + tempn)
                            nextTimeStep+=microbes[tempId].infectedAt + tempInitialStr - 1;
                        else
                            nextTimeStep+=microbes[tempId].infectedAt + tempInitialStr;
                    }
                    else
                        nextTimeStep += tempn;
                }
                Microbe tempMicrobe(tempId,tempInitialStr,tempCurrStr,tempn,tempInfectedAt);
                if (nextTimeStep<=endTime){
                    if (tempId==microbes.size())
                        microbes.push_back(tempMicrobe);
                    else
                        microbes[tempId]=tempMicrobe;
                    TimeEvent tempEvent;
                    tempEvent.timeStep = nextTimeStep;
                    tempEvent.microbe = &microbes[tempId];
                    eventHeap.insert(tempEvent);
                    cout<<"next scheduled due to new one at "<<nextTimeStep<<endl;
                }
                currTime.microbe->currStr--; //BEGIN insert the next time event for parent
                if (((currTime.timeStep+currTime.microbe->n)>=currTime.microbe->infectedAt) &&
                        (currTime.microbe->infectedAt>=0)){
                    currTime.timeStep = currTime.microbe->infectedAt + currTime.microbe->currStr;
                    currTime.microbe->currStr==0;
                }
                else
                    currTime.timeStep += currTime.microbe->n;
                if (currTime.timeStep<=endTime){
                    eventHeap.insert(currTime);
                    cout<<"inserted parent's copy id "<<currTime.microbe->id<<" at next "<<currTime.timeStep<<endl;
                }
            }
            eventHeap.deleteMin();
            currTime = eventHeap.getMin();
        }
    }
    return 0;
}
