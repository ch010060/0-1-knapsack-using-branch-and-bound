//The 0/1 Knapsack Problem
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <queue>

#define Bag_Size_MAX 5000000
#define Item_Num_MAX 1000

using namespace std;

int Bag_Size;  //背包大小
int Item_count;//物品數量

struct Item
{
    int weight;//物品重量
    int value;//物品價值
};
 
struct Node
{
    int height;//位於此樹的層數(樹高)
	int value;//此node的價值
	int bound;//(從u到)此node的upper bound
    int weight;//此node的權重
};
 
//比較value/weight的大小
bool MyCompare(Item i, Item j)
{
    return (double)i.value/i.weight > (double)j.value/j.weight;
}

//初始化
template<typename T,size_t N>
void Init(T (&ItemList)[N])
{
	//先依照value/weight的大小，由大排到小(先取單位價值最高的)
	//若沒有先排序，就不能用greedy method處理了
	sort(ItemList,ItemList+Item_count,MyCompare);
}

//塞東西進背包(依照單位價值高的先塞)
int Add_Item(int *lower_bound,int *upper_bound,int Item_num,int *weight_sum,Item ItemList[])
{
    while ((Item_num < Item_count) && (*weight_sum + ItemList[Item_num].weight <= Bag_Size))
    {
        *weight_sum = *weight_sum + ItemList[Item_num].weight;
        *upper_bound = *upper_bound + ItemList[Item_num].value;
        Item_num++;
		if(*weight_sum > Bag_Size)
			*lower_bound = 0;
    }

	return Item_num;
}

//檢查背包是否塞完全部物品
int Check_full(int *lower_bound,int upper_bound,int Item_num,int weight_sum,Item ItemList[])
{
	//如果背包重量已滿，但還沒塞進全部物品
	//Upper bound需考慮最後一個不能塞的Item其單位價值
    if (Item_num < Item_count){
		*lower_bound = upper_bound;
        upper_bound = upper_bound + (Bag_Size - weight_sum) * ItemList[Item_num].value / ItemList[Item_num].weight;
	}
	return upper_bound;
}

//檢查是否更新upper bound
int Check_bound(Node current, Item ItemList[],int *lower_bound)
{
	//先檢查有無超過背包限制重量
    if (current.weight >= Bag_Size)
        return 0;

    //初始化
    int upper_bound = current.value;
    int Item_num = current.height + 1;
    int weight_sum = current.weight; 

	//開始塞背包，回傳最後不能塞進的Item編號
	Item_num = Add_Item(lower_bound,&upper_bound,Item_num,&weight_sum,ItemList);

	//依照背包是否塞完全部物品，來更新最後的upper_bound
	upper_bound = Check_full(lower_bound,upper_bound,Item_num,weight_sum,ItemList);

    return upper_bound;
}

//檢查是否更新最大價值
int Check_value(Node current,int Max_value)
{
    if (current.weight <= Bag_Size && current.value > Max_value)//如果符合條件又更有價值
           Max_value = current.value;
	return Max_value;
}
 
//回傳最大價值
int knapsack(Item ItemList[])
{ 
    //初始化
    queue<Node> Bag;
    Node current, next;
	int Max_value = 0;
    current.height = -1;//未建樹前為-1,樹根為0(配合矩陣的index從0開始)
    current.value = 0;
	current.weight = 0;
	int lower_bound = 0,lb=0;

    Bag.push(current);
    while (!Bag.empty())
    {
        //從背包取物
        current = Bag.front();
        Bag.pop();
 
        //檢查是否為初始node
        if (current.height == -1)
            next.height = 0;
 
        //產生下一個node，計算其所有子node的總價值
        next.height = current.height + 1;//從下一層開始
		next.value = current.value + ItemList[next.height].value;//原本的價值+下一個物品的價值
        next.weight = current.weight + ItemList[next.height].weight;//原本的重量+下一個物品的重量
        next.bound = Check_bound(next,ItemList,&lower_bound);//計算下一個node的upper bound

		if(lower_bound > lb)
			lb = lower_bound;

		//檢查是否超過背包限制重量，以及決定是否更新當前最大價值
		Max_value = Check_value(next,Max_value);
		//如果下一個node的upper bound比原本的總價值更有價值，代表要塞進背包
        if (next.bound > Max_value && next.bound >= lower_bound)
            Bag.push(next);

		//計算下一個node本身的價值(把next當成current的temp來使用)
        next.value = current.value;
        next.weight = current.weight;
        next.bound = Check_bound(next,ItemList,&lower_bound);
		//檢查是否超過背包限制重量，以及決定是否更新當前最大價值
		Max_value = Check_value(next,Max_value);
		//如果當前node的upper bound比原本的總價值更有價值，代表要塞進背包
        if (next.bound > Max_value && next.bound > lb)
            Bag.push(next);
    }
 
    return Max_value;
}

int main()
{
/*	
	Item ItemList[] = {{41, 442}, {50, 525}, {49, 511},{59, 593}, {55, 546},{57,564},{60,617}};//物品清單
	Bag_Size = 170; 
	Item_count = sizeof(ItemList) / sizeof(ItemList[0]);
*/	

	Item ItemList[Item_Num_MAX]={0};//物品清單

	cin >> Bag_Size >> Item_count; 

	for(int i=0;i<Item_count;i++)
	{
		cin >> ItemList[i].value;
		cin >> ItemList[i].weight;
	}

	//初始化
	Init(ItemList);

	//主function
    cout << knapsack(ItemList);
 
	//system("PAUSE");
    return 0;
}