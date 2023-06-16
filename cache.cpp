#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <map>
#include <algorithm>
#include <bitset>
#include <bits/stdc++.h>
using namespace std;

struct Set{
    vector<unsigned long long int> last_used;
    
    vector<bool> valid_bit;
    vector<bool> dirty_bit;
    vector<unsigned long long int> tag;

};
unsigned long long int memaccess=0;
struct Cache{
    vector<Set> set;
    unsigned long long int associativity;
    unsigned long long int blocksize;
    unsigned long long int num_sets;
    unsigned long long int cachesize;
    unsigned long long int readhits;
    unsigned long long int readmiss;
    unsigned long long int writehits;
    unsigned long long int writemiss;
    unsigned long long int num_access;
    unsigned long long int writebacks;
    unsigned long long int read;
    unsigned long long int write;
    

}LC1,LC2;

unsigned long long int access_time;
void dirtyL1(unsigned long long int lruadd){
    LC2.num_access+=1;
    unsigned long long int index = (lruadd / LC2.blocksize) % (LC2.num_sets);
    unsigned long long int tag = lruadd / (LC2.blocksize*LC2.num_sets);
    for (unsigned long long int i = 0; i< LC2.associativity; i++){
        if (LC2.set[index].tag[i]==tag && LC2.set[index].valid_bit[i]==true){
            
                LC2.set[index].dirty_bit[i]=true;
                LC2.set[index].last_used[i] = LC2.num_access;
        }
    }
}
void cleanL2(unsigned long long int lruadd){
    access_time+=1;
    LC1.num_access+=1;
    unsigned long long int index = (lruadd / LC1.blocksize) % (LC1.num_sets);
    unsigned long long int tag = lruadd / (LC1.blocksize*LC1.num_sets);
    for (unsigned long long int i = 0; i< LC1.associativity; i++){
        if (LC1.set[index].tag[i]==tag && LC1.set[index].valid_bit[i]==true){
                if(LC1.set[index].dirty_bit[i]==true){
                    LC1.readhits+=1;
                    LC2.write+=1;
                                        LC2.writehits++;
                                        LC2.writebacks++;
                                        access_time+=220;
                                        memaccess+=1;
                }
                LC1.set[index].valid_bit[i]=false;
                LC1.set[index].last_used[i] = LC1.num_access;
                

        }
    }

}
void initiate_cache(int associativity1, int associativity2 ,int blocksize, int cachesize1, int cachesize2){
    LC1.cachesize= cachesize1;
    LC2.cachesize= cachesize2;
    LC1.num_sets = cachesize1/(associativity1*blocksize);
    LC2.num_sets = cachesize2/(associativity2*blocksize);
    LC1.associativity = associativity1;
    LC2.associativity = associativity2;
    LC1.blocksize = blocksize;
    LC2.blocksize = blocksize;
    LC1.cachesize = cachesize1;
    LC2.cachesize = cachesize2;
    LC1.readmiss = 0;
    LC1.readhits = 0;
    LC1.writemiss = 0;
    LC1.writehits = 0;
    LC2.readmiss = 0;
    LC2.readhits = 0;
    LC2.writemiss = 0;
    LC2.writehits = 0;
    LC1.read=0;
    LC1.write=0;
    LC2.read=0;

    LC2.write=0;
    LC2.writebacks = 0;
    LC1.num_access = 0;
    LC2.num_access = 0;
    for(int i=0;i<LC1.num_sets;i++){
        Set temp;
        
        for(int j=0;j<LC1.associativity;j++){
            temp.last_used.push_back(0);
 
            temp.valid_bit.push_back(false);
            temp.dirty_bit.push_back(false);
            temp.tag.push_back(0);
        }
        LC1.set.push_back(temp);
    }
    for(int i=0;i<LC2.num_sets;i++){
        Set temp;
        
        for(int j=0;j<LC2.associativity;j++){
            temp.last_used.push_back(0);

            temp.valid_bit.push_back(false);
            temp.dirty_bit.push_back(false);
            temp.tag.push_back(0);
        }
        LC2.set.push_back(temp);
    }
    
}
// getIndex function is taken as it is from geekforgeeks website.
unsigned long long int getIndex(vector<unsigned long long int> v, unsigned long long int K)
{
    auto it = find(v.begin(), v.end(), K); 

    if (it != v.end()) 
    {

        int index = it - v.begin();
        return index;
    }
    else{
        return 0;
    }    
}

void readfunctL1(unsigned long long int address,unsigned long long int ans[2]){
    access_time+=1;
    LC1.read++;
    unsigned long long int index = (address / LC1.blocksize) % (LC1.num_sets);
    unsigned long long int tag = address / (LC1.blocksize*LC1.num_sets);
    LC1.num_access++;
    int lrucount=0;
    bool readL1=false;
    unsigned long long int store;
    for (unsigned long long int i = 0; i< LC1.associativity; i++){
        if (LC1.set[index].tag[i]==tag && LC1.set[index].valid_bit[i]==true){
           
                LC1.readhits++;
                readL1=true;
                ans[0]=0;
                ans[1]=0;
                
                LC1.set[index].last_used[i] = LC1.num_access;           
        }
        
        for (unsigned long long int i = 0; i<LC1.associativity;i++){
            if (LC1.set[index].valid_bit[i]==true){
            lrucount++;
            }
            else{
                store=i;
            }
        }

    }
    if(not readL1){
        
        LC1.readmiss++;
        ans[0]=1;
        ans[1]=address;
        if(lrucount<LC1.associativity){
            
            
            LC1.set[index].valid_bit[store]=true;
            LC1.set[index].last_used[store]=LC1.num_access;
            
            LC1.set[index].tag[store]=tag;
            LC1.set[index].dirty_bit[store]=false;
            

        }
        else {
        unsigned long long int  min_access = *min_element(LC1.set[index].last_used.begin(),LC1.set[index].last_used.end());
        unsigned long long int lruindex = getIndex(LC1.set[index].last_used,min_access);
        unsigned long long int taglru=LC1.set[index].tag[lruindex];
        
        
        if(LC1.set[index].dirty_bit[lruindex]==true){
            access_time+=20;
            dirtyL1(taglru*LC1.blocksize*LC1.num_sets+index*LC1.blocksize);
            LC1.set[index].dirty_bit[lruindex]=false;
            LC1.set[index].tag[lruindex]=tag;
            LC1.set[index].last_used[lruindex]=LC1.num_access;
            LC1.set[index].valid_bit[lruindex]=true;
            LC2.writehits++;
            LC2.write++;
        }
        else{
            LC1.set[index].dirty_bit[lruindex]=false;
            LC1.set[index].tag[lruindex]=tag;
            LC1.set[index].last_used[lruindex]=LC1.num_access;
            LC1.set[index].valid_bit[lruindex]=true;
        }
        }

        }
        
    }
void writefunctL1(unsigned long long int address, unsigned long long int ans[2]){
    access_time+=1;
    LC1.write++;
    unsigned long long int index = (address / LC1.blocksize) % (LC1.num_sets);
    unsigned long long int tag = address / (LC1.blocksize*LC1.num_sets);
    LC1.num_access++;
    int lrucount=0;
    bool writeL1=false;
    unsigned long long int store;
    for (unsigned long long int i = 0; i< LC1.associativity; i++){
        if (LC1.set[index].tag[i]==tag && LC1.set[index].valid_bit[i]==true){
                ans[0]=0;
                ans[1]=0;
                LC1.writehits++;
                writeL1=true;
                LC1.set[index].dirty_bit[i]=true;
                LC1.set[index].last_used[i] = LC1.num_access;           
        }
        
        for (unsigned long long int i = 0; i<LC1.associativity;i++){
            if (LC1.set[index].valid_bit[i]==true){
            lrucount++;
            }
            else{
                store=i;
            }
        }

    }
    if(not writeL1){
        LC1.writemiss++;
        ans[0]=1;
        ans[1]=address;
        if(lrucount<LC1.associativity){
            
            
            LC1.set[index].valid_bit[store]=true;
            LC1.set[index].last_used[store]=LC1.num_access;
            LC1.set[index].tag[store]=tag;
            LC1.set[index].dirty_bit[store]=true;
            

        }
        else {
        unsigned long long int  min_access = *min_element(LC1.set[index].last_used.begin(),LC1.set[index].last_used.end());
        unsigned long long int lruindex = getIndex(LC1.set[index].last_used,min_access);
        unsigned long long int taglru=LC1.set[index].tag[lruindex];
        
        if(LC1.set[index].dirty_bit[lruindex]==true){
            
            access_time+=20;
            dirtyL1(taglru*LC1.blocksize*LC1.num_sets+index*LC1.blocksize);
            LC1.set[index].dirty_bit[lruindex]=true;
            LC1.set[index].tag[lruindex]=tag;
            LC1.set[index].last_used[lruindex]=LC1.num_access;
            LC1.set[index].valid_bit[lruindex]=true;
            LC2.write++;
            LC2.writehits++;
        }
        else{
            LC1.set[index].dirty_bit[lruindex]=true;
            LC1.set[index].tag[lruindex]=tag;
            LC1.set[index].last_used[lruindex]=LC1.num_access;
            LC1.set[index].valid_bit[lruindex]=true;
        }
        }

        }
        
    }

void readfunctL2(unsigned long long int address){
    LC2.read++;
    access_time+=20;
    unsigned long long int index = (address / LC2.blocksize) % (LC2.num_sets);
    unsigned long long int tag = address / (LC2.blocksize*LC2.num_sets);
    LC2.num_access++;
    int lrucount=0;
    bool readL2=false;
    unsigned long long int store;
    for (unsigned long long int i = 0; i< LC2.associativity; i++){
        if (LC2.set[index].tag[i]==tag && LC2.set[index].valid_bit[i]==true){
           
                LC2.readhits++;
                readL2=true;
 
                
                LC2.set[index].last_used[i] = LC2.num_access;           
        }
        
        for (unsigned long long int i = 0; i<LC2.associativity;i++){
            if (LC2.set[index].valid_bit[i]==true){
            lrucount++;
            }
            else{
                store=i;
            }
        }

    }
    if(not readL2){
        LC2.readmiss++;
        if(lrucount<LC2.associativity){
            
            
            LC2.set[index].valid_bit[store]=true;
            LC2.set[index].last_used[store]=LC2.num_access;
            access_time+=200;
            memaccess+=1;
            LC2.set[index].tag[store]=tag;
            LC2.set[index].dirty_bit[store]=false;
            

        }
        else {
        unsigned long long int  min_access = *min_element(LC2.set[index].last_used.begin(),LC2.set[index].last_used.end());
        unsigned long long int lruindex = getIndex(LC2.set[index].last_used,min_access);
        unsigned long long int taglru=LC2.set[index].tag[lruindex];        
        unsigned long long ans=taglru*LC2.blocksize*LC2.num_sets+index*LC2.blocksize;
        
        
        if(LC2.set[index].dirty_bit[lruindex]==true){
            memaccess+=2;
            access_time+=400;
            LC2.writebacks++;
            LC2.set[index].dirty_bit[lruindex]=false;
            LC2.set[index].tag[lruindex]=tag;
            LC2.set[index].last_used[lruindex]=LC2.num_access;
            LC2.set[index].valid_bit[lruindex]=true;

        }
        else{
            memaccess+=1;
            access_time+=200;
            cleanL2(ans);
            LC2.set[index].dirty_bit[lruindex]=false;
            LC2.set[index].tag[lruindex]=tag;
            LC2.set[index].last_used[lruindex]=LC2.num_access;
            LC2.set[index].valid_bit[lruindex]=true;
        }
        }

        }
    
}

void read(unsigned long long int address){
    unsigned long long int ans[2];
    readfunctL1(address, ans);
    if(ans[0]==1){
        readfunctL2(ans[1]);
    }
}
void write(unsigned long long int address){
    unsigned long long int ans[2];
    writefunctL1(address, ans);
    if(ans[0]==1){
        readfunctL2(ans[1]);
    }
}
int main(int argc, char *argv[]){
     if (argc != 7)
        {
                std::cerr << "Required argument\n";
                return 0;
        }
    unsigned long long blksize=atoi(argv[1]);
    unsigned long long int cachesizeL1=atoi(argv[2]);
    unsigned long long int assocL1=atoi(argv[3]);
    unsigned long long int cachesizeL2=atoi(argv[4]);
    unsigned long long int assocL2=atoi(argv[5]);
    initiate_cache(assocL1,assocL2,blksize,cachesizeL1,cachesizeL2);
        std::ifstream file(argv[6]);
    if (file.is_open()){
        std::string line;
        while (getline(file, line)){
            std::istringstream iss(line);
            std::string word;
            iss >> word;
            if(word=="#eof"){
                break;
            }
            else if(word=="r"){
                unsigned long long int address;
                iss >> std::hex >> address;
                read(address);
            }
            else if(word=="w"){
                unsigned long long int address;
                iss >> std::hex >> address;
                write(address);
            }
            else{
                std::cerr << "Invalid input\n";
                return 0;
            }
        }
        std::cout<<"===== Simulator Results ====="<<"\n";
        std::cout << "i. number of L1 reads:                   " << LC1.read << std::endl;
        std::cout << "ii. number of L1 read misses:            " << LC1.readmiss << std::endl;
        std::cout << "iii. number of L1 writes:                " << LC1.write << std::endl;
        std::cout << "iv. number of L1 write misses:           " << LC1.writemiss << std::endl;
        std::cout << "v. L1 miss rate:                         " <<std::fixed << std::setprecision(4) <<  ((float)LC1.readmiss + (float)LC1.writemiss)/((float)LC1.read + (float)LC1.write)<< std::endl;
        std::cout << "vi. number of writebacks from L1 memory: " << LC2.writehits << std::endl;
        std::cout << "vii. number of L2 reads:                 " << LC2.read << std::endl;
        std::cout << "viii. number of L2 read misses:          " << LC2.readmiss << std::endl;
        std::cout << "xi. number of L2 writes:                 " << LC2.writehits << std::endl;
        std::cout << "x. number of L2 write misses:            " << 0 << std::endl;
        std::cout << "xi. L2 miss rate:                        " << std::fixed << std::setprecision(4) << ((float)LC2.readmiss )/((float)LC2.read + (float)LC2.writehits)<< std::endl;
        std::cout << "xii. number of writebacks from L1 memory:" << LC2.writebacks << std::endl;
        return 0;
    }
                
    else
        {
                std::cerr << "File could not be opened. Terminating...\n";
                return 0;
        }


}
// std::fixed << std::setprecision(4) << 