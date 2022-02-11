#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
using std::swap;

const int Maxn = 10000 + 5 ;
const int pointRange = 100 ;
const int maxIter = 500 ;
const int nInit = 10 ;
const int INF = 0x7f7f7f7f ;
const double EPS = 0.01 ;

class Point2D {

    double x,y;

public:
    Point2D(double readX=0,double readY=0) { x=readX; y=readY; }

    double  clusterDistance(Point2D anotherPoint) {
        return sqrt((x-anotherPoint.x)*(x-anotherPoint.x)+(y-anotherPoint.y)*(y-anotherPoint.y));
    }

    Point2D& operator = (const Point2D& anotherPoint) {
        x=anotherPoint.x;
        y=anotherPoint.y;
        return *this;
    }

    Point2D& operator += (const Point2D& anotherPoint) {
        x+=anotherPoint.x;
        y+=anotherPoint.y;
        return *this;
    }

    Point2D& operator /= (const int& num) {
        x/=num;
        y/=num;
        return *this;
    }

    double operator - (const Point2D& other) {
        return abs(x-other.x)+abs(y-other.y);
    }

    void read() {
        scanf("%lf%lf",&x,&y);
    }

    void print() {
        printf("(%.3f,%.3f)",x,y);
    }

};
typedef Point2D Cluster;

double creatRand() {                   //产生范围在(-pointRange,pointRange)之间的小数
    int num=rand()%pointRange;
    double decimal=(rand()%1000)/1000.0;
    if(rand()&1) return -num-decimal;   
    else return num+decimal;
}

void dataCreat(Point2D *points,int Size) {
    for(register int i=0;i<Size;++i) {
        points[i]=Point2D(creatRand(),creatRand());
    }
}

Cluster* kMeans(Point2D *points,int size,int nClusters) {
    Cluster *centers=(Cluster *)malloc(nClusters*sizeof(Point2D));
    Cluster *centersTemporary=(Cluster *)malloc(nClusters*sizeof(Point2D));
    //一个记录答案,另一个负责滚动
    int *clustersSize=(int *)malloc(nClusters*sizeof(int));
    //记录对应簇中的点的个数
    dataCreat(centers,nClusters);            //随机产生簇心
    for(int Dai=0;Dai<maxIter;Dai++) { 
        double eps=0;    
        memset(centersTemporary,0,nClusters*sizeof(Cluster));
        memset(clustersSize,0,nClusters*sizeof(int));     
        for(int i=1;i<=size;i++) {           //枚举每个点找到族心
            int clusterBelonging=0;
            double distance=INF;
            for(int eachCluster=0;eachCluster<nClusters;eachCluster++) {  //枚举每个簇
                double nowDistance=centers[eachCluster].clusterDistance(points[i]);
                if(nowDistance<distance) { 
                    distance=nowDistance;
                    clusterBelonging=eachCluster;
                }
            }
            centersTemporary[clusterBelonging]+=points[i];
            clustersSize[clusterBelonging]++;                 //累加所有点
        }
        for(int eachCluster=0;eachCluster<nClusters;eachCluster++) {
            if(clustersSize[eachCluster]>0) {
                centersTemporary[eachCluster]/=clustersSize[eachCluster];  //求质心，可能刚开始该簇中无点，需特判
            }
            else centersTemporary[eachCluster]=Cluster(creatRand(),creatRand());  //重新生成簇点
            eps+=(centers[eachCluster]-centersTemporary[eachCluster]);     //计算与上一次的差距
        }
        memcpy(centers,centersTemporary,nClusters*sizeof(Cluster));        //滚动
        if(eps<EPS) break;                                                 //与上次差距小直接退出
    }
    free(centersTemporary);
    free(clustersSize);
    return centers;
}

double estimate(Point2D *points,int size,Cluster *centers,int nCluster) {  //同理，计算所求的质点与对应簇中的点的总距离
    double ans=0;
    for(int i=0;i<size;i++) {
        int clusterBelonging=0;
        double distance=INF;
        for(int eachCluster=0;eachCluster<nCluster;eachCluster++) {
            double nowDistance=centers[eachCluster].clusterDistance(points[i]);
            if(nowDistance<distance) { 
                distance=nowDistance;
                clusterBelonging=eachCluster;
            }
        }
        ans+=distance;    
    }
    return ans;
}

Point2D testPoints[Maxn];
int main() {
    /*freopen("test.in","r",stdin);
    freopen("test.out","w",stdout);*/
    srand(time(NULL));
    int n,k;
    scanf("%d%d",&n,&k);  //总点数和k值
    /*dataCreat(testPoint,n);
    for(int i=0;i<n;++i) {
        printf("Point %d:\n",i);
        testPoint[i].print();
        putchar('\n');
    }*/
    for(int i=0;i<n;i++) {
        testPoints[i].read();
    }   //手动输入数据
    Cluster *ansCenters=kMeans(testPoints,n,k);
    double ans=INF,ansTemporary;
    for(int i=0;i<nInit;i++) {
        Cluster* centersTemporary=kMeans(testPoints,n,k);
        ansTemporary=estimate(testPoints,n,centersTemporary,k);
        if(ansTemporary<ans) {
            ans=ansTemporary;
            swap(ansCenters,centersTemporary);
        }
        free(centersTemporary);
        //迭代，选择最优方案
    }
    printf("Estimate: %.1f\n",ans);
    for(int i=0;i<k;i++) {
        printf("Centers %d:",i);
        ansCenters[i].print();
        putchar('\n');
    }
    free(ansCenters);
    return 0;
}