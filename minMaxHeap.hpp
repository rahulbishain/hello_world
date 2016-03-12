#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

template <class T>
class Predicate{
    public:
        virtual bool toDelete(T a){return false;}
};

template <class T>
class MinMaxHeap{
    private:
        vector <T> A;
        int heapSize;
        bool lastLevelMin;		

    public:
        MinMaxHeap(){
            heapSize = 0;
            lastLevelMin = true;	//indicates last level of heap is min or max
            T temp;			//insertion starts from index 1 for ease of calculation
            A.push_back(temp);
        }

        void insert(T elem){
            int i = heapSize+1;
            if (i>=A.size())
                A.push_back(elem);
            else 
                A[i]=elem;
            heapSize++;
            //cout<<"0000. called to insert"<<elem<<" @ "<<heapSize<<endl;
            if (i>>1){			//if parent exists
                if ((lastLevelMin ^ (elem<A[i>>1]))){		//... and it is not greater(less) than the new insertion for min(max)level, swap
                    swap(i>>1,i);
                    bubbleUp(i>>1,!lastLevelMin);}//start comparing only max(min) ancestors
                else				//... if it is greater(less) than new insertion,
                    bubbleUp(i,lastLevelMin);	//... then compare only in min(max) ancestors
            }
            if (floor(log2(heapSize+1)) - floor(log2(heapSize)) == 1)		//change level flag lastLevelMin if current level is full
                lastLevelMin = !lastLevelMin;
            //cout<<"0000. inserted "<<elem<<" at "<<i<<endl;
            //cout<<"0000. level flag is"<<(lastLevelMin?"min":"max")<<endl;
        }

        T getMin(){
            if (heapSize != 0)
                return A[1];
        }

        T getMax(){
            if(heapSize == 1)
                return A[1];
            else if (heapSize == 2)
                return A[2];
            else if (heapSize >= 3)
                return (A[2]<A[3]?A[3]:A[2]);
        }

        void deleteMin(){
            if (heapSize != 0)
                deleteI(1);
        }

        void deleteMax(){
            if(heapSize == 1)
                deleteI(1);
            else if (heapSize == 2)
                deleteI(2);
            else if (heapSize >= 3)
                deleteI(A[2]<A[3]?3:2);
        }

        void deleteElems(Predicate<T> &obj){
            //cout<<"0000. called to delete at heapSize="<<heapSize<<endl;
            for(int i = 1;i <= heapSize;i++){
                //cout<<"0000. checking deletion of "<<A[i]<<" @ "<<i<<endl;
                if(obj.toDelete(A[i])){
                    //cout<<"0000. confirming deletion of "<<A[i]<<" @ "<<i<<endl;
                    deleteI(i);
                    i = 1;              //SCOPE FOR IMPROVEMENT: check if bubble up has happened.. if not,no need for this line
                }
            }
        }
        void swap(int a, int b){
            T temp;
            temp = A[a];
            A[a] = A[b];
            A[b] = temp;
            //cout<<"0000. swapped "<<A[b]<<" with "<<A[a]<<endl;
        }

        void bubbleUp(int a, bool levelFlag){
            //cout<<"0000. came to check for"<<A[a]<<endl;
            while (a>>2){
                if (!((levelFlag) ^ ((A[a]<A[a>>2])))) {
                    swap(a,a>>2);
                    a=a>>2;
                }
                else break;
            }
        }

        T dispElem(int a){
            //cout<<a<<" "<<A[a]<<endl;
        }

        void deleteI(int a){
            T temp1,temp;
            //cout<<"0000. heapsize before deletion ="<<heapSize<<endl;
            if ((a>=1)&&(a<=heapSize)){		//delete only if in range
                temp = A[heapSize];
                A[heapSize] = temp1;
                heapSize--;
                while (a<=heapSize){		//(A) try reconstructing heap DOWNWARD only if deleted node was not the root
                    A[a] = temp;
                    //cout<<"0000. inital val at "<<a<<" is "<<A[a]<<endl;
                    bool currLevelMin = isCurrLevelMin(a);
                    int toSwap = a;
                    if (a!=heapSize){
                        //cout<<"0000. inside a!=heapsize"<<endl;
                        if ((a<<1)==heapSize)	//1. if parent of only a leaf 
                            toSwap = a<<1;
                        else if (((a<<1)<=heapSize-1)&&((a<<2)>heapSize)){//2. If parent of 2 leaves and no grandchildren
                            if (currLevelMin ^ (A[a<<1] < A[(a<<1)+1]))//if this level is max then swap with max of the two children
                                toSwap = (a<<1) + 1;
                            else
                                toSwap = a<<1;
                        }
                        else if ((a<<2)<=heapSize){ //3. if grandparent and curr level is min (or max)
                            toSwap = findMinMax(a<<2,currLevelMin);	//... find min (or max) among the 4 grand-children
                            //cout<<"0000. grandchild found. swapping with "<<toSwap<<endl;
                        }
                        else if ((a<<1)>heapSize) break;//4. Stop if no children found. Deletion over

                        if (currLevelMin ^ (A[a]<A[toSwap]))
                            swap(a,toSwap);//Swap and proceed only if the next node can be replaced, else stop.
                        else break;
                        a=toSwap;
                    }
                    else break;			
                }
                bool currLevelMin = isCurrLevelMin(a);	//(B) if the replaced node did not move down, check if it needs to move up.
                if ((a>>1)&&(a<=heapSize)){			//if parent exists
                    if ((currLevelMin ^ (A[a]<A[a>>1]))){		//... and it is not greater(less) than the new insertion for min(max)level, swap
                        swap(a>>1,a);
                        bubbleUp(a>>1,!currLevelMin);}//start comparing only max(min) ancestors
                    else				//... if it is greater(less) than new insertion,
                        bubbleUp(a,currLevelMin);	//... then compare only in min(max) ancestors
                }
                //cout<<"0000. new heapsize post deletion is = "<<heapSize<<" and levelMin is "<<(lastLevelMin?"min":"max")<<endl;
                if (heapSize==0)
                    lastLevelMin = true;
                else if (floor(log2(heapSize+2)) - floor(log2(heapSize+1)) == 1)		//change level flag lastLevelMin if current level was full before this deletion
                    lastLevelMin = !lastLevelMin;
                //cout<<"0000. new last levelmin after deletion is "<<(lastLevelMin?"min":"max")<<endl;
            }
        }

        bool isCurrLevelMin(int a){					//current level of heap is min or max?
            //cout<<"0000. curr level "<<((int)floor(log2(a)+1)%2)<<endl;
            return ((int)floor(log2(a)+1)%2);
        }

        int findMinMax(int a, bool currLevelMin){
            int minMax = a;
            int uLimit = a + 4;//check only max 4 grand-children
            while ((++a<=heapSize)&&(a<uLimit)){ 
                //cout<<"0000. checking at "<<a<<" = "<<A[a]<<" with "<<minMax<<" for "<<(currLevelMin==1?"min":"max")<<endl;
                if ((!currLevelMin) ^ (A[a]<A[minMax])) minMax = a;}
            //cout<<"0000. final minmax at "<<minMax<<" = "<<A[minMax]<<endl;
            return minMax;
        }

        int getHeapSize() {
            return heapSize;
        }

};




/*int main (){

  MinMaxHeap <int>heap;
  heap.insert(10);
  heap.insert(11);
  heap.insert(5);
  heap.insert(13);
  heap.insert(19);
  heap.insert(22);
  heap.insert(9);
  heap.insert(8);
  heap.insert(25);
  heap.insert(7);
  heap.insert(2);

  for (int i=1;i<12;i++)
  heap.dispElem(i);
  heap.deleteI(9);

  for (int i=1;i<11;i++)
  heap.dispElem(i);

  }
  */
