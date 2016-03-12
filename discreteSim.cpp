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

        /*void operator=(const TimeEvent &m){	//NOT NEEDED
            timeStep = m.timeStep;
            microbe = m.microbe;
        }*/

        bool operator<(const TimeEvent &m){
            if (timeStep<m.timeStep)
                return true;
            else if (timeStep == m.timeStep)
                return (microbe->id<m.microbe->id);
            else 
                return false;
        }
};


void dispHeap(MinMaxHeap<TimeEvent> *heap){
    for (int i=1;i<=heap->getHeapSize();i++){
        TimeEvent temp = heap->dispElem(i);
        //cout<<i<<".timeStep="<<temp.timeStep;
        //cout<<"ID="<<temp.microbe->id;
        //cout<<"infectedAt"<<temp.microbe->infectedAt;
        //cout<<"currstr="<<temp.microbe->currStr<<"++++";
    }
    //cout<<endl;
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
        int tempTime = infectionTimeInp[i][1];
        microbes[tempId].infectedAt = tempTime;
    }

    MinMaxHeap<TimeEvent> eventHeap;
    TimeEvent tempEvent;
    for (int i=0;i<n;i++){
        microbes[i].id = i;
        microbes[i].initialStr = strInp[i];
        microbes[i].currStr = strInp[i];
        microbes[i].n = (((i+strInp[i])*29)%37+7);
        tempEvent.microbe = &microbes[i];
        if (microbes[i].n>=microbes[i].infectedAt && microbes[i].infectedAt>=0){
            microbes[i].currStr = 0;
            tempEvent.timeStep =  microbes[i].infectedAt + (microbes[i].n==microbes[i].infectedAt?microbes[i].initialStr--:microbes[i].initialStr);
        }
        else
            tempEvent.timeStep = microbes[i].n;
        if (tempEvent.timeStep<=endTime){
            eventHeap.insert(tempEvent);
            //cout<<"inserted in heap id "<<microbes[i].id<<endl;
            //cout<<"inserted in heap id "<<tempEvent.microbe->id<<endl;
            //cout<<"inserted in heap strength "<<microbes[i].initialStr<<endl;
            //cout<<"inserted in heap n "<<microbes[i].n<<endl;
            //cout<<"inserted in heap infectedAt "<<microbes[i].infectedAt<<endl;
        }
    }

    //cout<<"allotted "<<eventHeap.getHeapSize()<<endl;
    dispHeap(&eventHeap);
    //Simulation
        TimeEvent currEvent;//holds the current time being simulated 
        currEvent=eventHeap.getMin();
        while(eventHeap.getHeapSize()>0 && currEvent.timeStep<=endTime){
            //cout<<"checking for curr time = "<<currEvent.timeStep<<" endTime="<<endTime<<" id="<<currEvent.microbe->id<<endl;
            if (currEvent.microbe->currStr==0)
                cout<<"d "<<currEvent.microbe->id<<" "<<currEvent.timeStep<<endl;
            else{
                int tempId = ++lastMicrobeIndex;
                int tempInitialStr = currEvent.microbe->initialStr;
                int tempCurrStr = tempInitialStr;
                int tempn = (((tempId + tempInitialStr)*29)%37+7);
                int tempInfectedAt = -1;
                int nextTimeStep = currEvent.timeStep;
                cout<<"b "<<tempId<<" "<<currEvent.timeStep<<endl;
                if (tempId==microbes.size()){
                    nextTimeStep+=tempn;
                    //cout<<"needed a new ID="<<tempId<<" next timestep = "<<nextTimeStep<<endl;
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
                    //cout<<"next scheduled due to new one at "<<nextTimeStep<<endl;
                }
                TimeEvent tempEvent=currEvent;//BEGIN insert the next time event for parent
                //cout<<"trying to insert parent id="<<tempEvent.microbe->id<<" currEvent="<<tempEvent.timeStep<<" n="<<tempEvent.microbe->n<<" infectedAt="<<tempEvent.microbe->infectedAt<<endl; 
                if (((currEvent.timeStep+currEvent.microbe->n)>=currEvent.microbe->infectedAt) &&
                        (currEvent.microbe->infectedAt>=0)){
                    tempEvent.timeStep = currEvent.microbe->infectedAt + currEvent.microbe->currStr - 1;
                    tempEvent.microbe->currStr=0;
                    //cout<<"found infection, inserting dead event for "<<tempEvent.microbe->id<<" at "<<tempEvent.timeStep<<" strength reduced to "<<tempEvent.microbe->currStr<<endl;
                }
                else{
                    tempEvent.timeStep += currEvent.microbe->n;
                    tempEvent.microbe->currStr--;
                    //cout<<"inserting next birthevent for "<<tempEvent.microbe->id<<" at "<<tempEvent.timeStep<<" strength "<<tempEvent.microbe->currStr<<endl;
                }
                if (tempEvent.timeStep<=endTime){
                    eventHeap.insert(tempEvent);
                    //cout<<"inserted parent's copy id "<<tempEvent.microbe->id<<" at next "<<tempEvent.timeStep<<endl;
                }
            }
            dispHeap(&eventHeap);
            eventHeap.deleteMin();
            //cout<<"new size="<<eventHeap.getHeapSize()<<endl;
            dispHeap(&eventHeap);
            currEvent = eventHeap.getMin();
        }
    return 0;
}
