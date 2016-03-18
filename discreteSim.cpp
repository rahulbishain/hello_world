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

        /*
        void operator=(const Microbe &m){
            id = m.id;
            initialStr = m.initialStr;
            currStr = m.currStr;
            n = m.n;
            infectedAt = m.infectedAt;
        }
        */

        bool operator<(const Microbe &m){
            return (id<m.id);
        }
};

class TimeEvent{
    public:
        int timeStep;
        int microbeId;	//MAKE THE < OPERATOR FOR MICROBES and TIMEEVENT

        /*void operator=(const TimeEvent &m){	//NOT NEEDED
            timeStep = m.timeStep;
            microbe = m.microbe;
        }*/

        bool operator<(const TimeEvent &m){
            if (timeStep<m.timeStep)
                return true;
            if (timeStep == m.timeStep)
                return (microbeId<m.microbeId);
            return false;
        }
};


void dispHeap(MinMaxHeap<TimeEvent> *heap,vector<Microbe> microbes){
    for (int i=1;i<=heap->getHeapSize();i++){
        TimeEvent temp = heap->dispElem(i);
        //cout<<i<<".timeStep="<<temp.timeStep;
        //cout<<"ID="<<temp.microbeId;
        //cout<<"infectedAt"<<microbes[temp.microbeId].infectedAt;
        //cout<<"currstr="<<microbes[temp.microbeId].currStr<<endl;
    }
}

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
        int infectedTime = infectionTimeInp[i][1];
        microbes[tempId].infectedAt = infectedTime;
        //cout<<"id="<<microbes[tempId].id<<" infectsAT"<<microbes[tempId].infectedAt;
    }
    //cout<<endl;
    MinMaxHeap<TimeEvent> eventHeap;
    TimeEvent tempEvent;
    for (int i=0;i<n;i++){
        microbes[i].id = i;
        microbes[i].initialStr = strInp[i];
        microbes[i].currStr = strInp[i];
        microbes[i].n = (((i+strInp[i])*29)%37+7);
        tempEvent.microbeId = i;
        if (microbes[i].n>=microbes[i].infectedAt && microbes[i].infectedAt>=0){
            microbes[i].currStr = 0;
            tempEvent.timeStep = microbes[i].infectedAt + microbes[i].initialStr;
            // tempEvent.timeStep =  microbes[i].infectedAt + (microbes[i].n==microbes[i].infectedAt?microbes[i].initialStr--:microbes[i].initialStr);
        }
        else
            tempEvent.timeStep = microbes[i].n;
        // if (tempEvent.timeStep<=endTime){
        eventHeap.insert(tempEvent);
            ////cout<<"inserted in heap id "<<microbes[i].id<<endl;
            ////cout<<"inserted in heap id "<<microbes[tempEvent.microbeId].id<<endl;
            ////cout<<"inserted in heap strength "<<microbes[i].initialStr<<endl;
            ////cout<<"inserted in heap n "<<microbes[i].n<<endl;
            ////cout<<"inserted in heap infectedAt "<<microbes[i].infectedAt<<endl;
        // }
    }

    ////cout<<"allotted "<<eventHeap.getHeapSize()<<endl;
    //dispHeap(&eventHeap,microbes);
    //Simulation
        TimeEvent currEvent;//holds the current time being simulated 
        currEvent=eventHeap.getMin();
        while(eventHeap.getHeapSize()>0 && currEvent.timeStep<=endTime){
            //cout<<"checking for curr time = "<<currEvent.timeStep<<" endTime="<<endTime<<" id="<<microbes[currEvent.microbeId].id<<" currStr="<<microbes[currEvent.microbeId].currStr<<" infect@"<<microbes[currEvent.microbeId].infectedAt<<" initialStr="<<microbes[currEvent.microbeId].initialStr<<endl;
            if (microbes[currEvent.microbeId].currStr==0)
                cout<<"d "<<microbes[currEvent.microbeId].id<<" "<<currEvent.timeStep<<endl;
            else{
                int tempId = ++lastMicrobeIndex;
                int tempInitialStr = microbes[currEvent.microbeId].initialStr;
                int tempCurrStr = tempInitialStr;
                int tempn = (((tempId + tempInitialStr)*29)%37+7);
                int tempInfectedAt = -1;
                int nextTimeStep = currEvent.timeStep;
                cout<<"b "<<tempId<<" "<<currEvent.timeStep<<endl;
                //cout<<"infectionAt "<<microbes[tempId].infectedAt<<endl;
                if (tempId==microbes.size()){
                    nextTimeStep+=tempn;
                    //cout<<"inserting new microbe vector @"<<microbes.size()<<endl;
                    Microbe tempMicrobe(tempId,tempInitialStr,tempCurrStr,tempn,tempInfectedAt);
                    microbes.push_back(tempMicrobe);
                }
                else{
                    if ((microbes[tempId].infectedAt<=nextTimeStep + tempn)&&(microbes[tempId].infectedAt>0)){
                        tempCurrStr = 0;
                        tempInfectedAt = microbes[tempId].infectedAt;
                        nextTimeStep = (tempInfectedAt + tempInitialStr);
                        //cout<<"child with old ID="<<tempId<<"infect at "<<tempInfectedAt<<" next timestep = "<<nextTimeStep<<endl;
                    }
                    else{
                        nextTimeStep += tempn;
                        //cout<<"child with old ID="<<tempId<<" next timestep = "<<nextTimeStep<<endl;
                    }
                    Microbe tempMicrobe(tempId,tempInitialStr,tempCurrStr,tempn,microbes[tempId].infectedAt);
                    microbes[tempId]=tempMicrobe;
                }
                TimeEvent tempEvent;
                tempEvent.timeStep = nextTimeStep;
                tempEvent.microbeId = tempId;
                //cout<<"going to insert child in heap"<<endl;
                eventHeap.insert(tempEvent);
                //dispHeap(&eventHeap,microbes);
                // //cout<<"next scheduled due to new one at "<<nextTimeStep<<endl;
                tempEvent=currEvent;//BEGIN insert the next time event for parent
                // //cout<<"trying to insert parent id="<<microbes[tempEvent.microbeId].id<<" currEvent="<<tempEvent.timeStep<<" n="<<microbes[tempEvent.microbeId].n<<" infectedAt="<<microbes[tempEvent.microbeId].infectedAt<<endl; 
                if (((currEvent.timeStep+microbes[currEvent.microbeId].n)>=microbes[currEvent.microbeId].infectedAt) &&
                        (microbes[currEvent.microbeId].infectedAt>=0)){
                    tempEvent.timeStep = microbes[currEvent.microbeId].infectedAt + microbes[currEvent.microbeId].currStr - 1;
                    microbes[tempEvent.microbeId].currStr=0;
                    ////cout<<"found infection, inserting dead event for "<<microbes[tempEvent.microbeId].id<<" at "<<tempEvent.timeStep<<" strength reduced to "<<microbes[tempEvent.microbeId].currStr<<endl;
                }
                else{
                    tempEvent.timeStep += microbes[currEvent.microbeId].n;
                    microbes[tempEvent.microbeId].currStr--;
                    // //cout<<"inserting next birthevent for "<<microbes[tempEvent.microbeId].id<<" at "<<tempEvent.timeStep<<" strength "<<microbes[tempEvent.microbeId].currStr<<endl;
                }
                //if (tempEvent.timeStep<=endTime){
                eventHeap.insert(tempEvent);
                // //cout<<"inserted parent's copy id "<<microbes[tempEvent.microbeId].id<<" at next "<<tempEvent.timeStep<<endl;
               // }
            }
            //dispHeap(&eventHeap,microbes);
            eventHeap.deleteMin();
             //cout<<"new size="<<eventHeap.getHeapSize()<<endl;
            //dispHeap(&eventHeap,microbes);
            currEvent = eventHeap.getMin();
        }
    cout<<eventHeap.getHeapSize()<<endl;
    return 0;
}
