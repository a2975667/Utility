#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

float recall(int topk, vector<int> &ans, vector<int> &groundt)
{
    int TP = 0, FN = 0, selected = groundt.size();

    vector<int>::iterator it = (groundt.begin() + topk);
    for (int j = 0; j < topk; j++)
    {
        if (std::find(groundt.begin(), it, ans[j]) != it)
            TP++;
    }

    if (selected > topk)
        selected = topk;

    FN = selected - TP;
    return TP * 1.0 / (TP + FN);
}

float precision(int topk, vector<int> &ans, vector<int> &groundt)
{
    int TP = 0, FP = 0, selected = ans.size();

    vector<int>::iterator it = (groundt.begin() + topk);
    for (int j = 0; j < topk; j++)
    {
        if (std::find(groundt.begin(), it, ans[j]) != it)
            TP++;
    }

    if (selected > topk)
        selected = topk;

    FP = selected - TP;
    return TP * 1.0 / (TP + FP);
}

float errorRatio(int topk, vector<float> &ans_dist, vector<float> &groundt_dist)
{
    if (ans_dist.size() == groundt_dist.size())
    {
        float counter = 0;
        for (int i = 0; i < topk; i++)
        {
            counter = ans_dist[i] / groundt_dist[i];
        }
        return counter / topk;
    }
    return -1.0;
}

int main(void)
{

    //read from file
    ifstream result, groundtruth;
    result.open("results.txt");
    groundtruth.open("groundtruth.txt");

    //read in parameter
    int entries, topK;
    result >> entries >> topK;
    vector<int> res, gt;
    vector<float> res_dist, gt_dist;
    res.resize(topK);
    gt.resize(topK);
    res_dist.resize(topK);
    gt_dist.resize(topK);

    string dummy;
    std::getline(result, dummy);
    cout << "Entries/TopK: " << entries << " " << topK << " \n\n";

    //to prevent huge file, compare line by line and output it directly

    for (int i = 0; i < entries; i++)
    {
        cout << "Top K for id: " << i << endl;
        string resultString = " .", groundString = " .";
        std::getline(result, resultString);
        std::getline(groundtruth, groundString);
        stringstream rs(resultString);
        stringstream gs(groundString);
        rs >> dummy;
        gs >> dummy;

        //save all entry into vector
        for (int j = 0; j < topK; j++)
        {
            rs >> res[j] >> res_dist[j];
            gs >> gt[j] >> gt_dist[j];
        }

        std::cout << "Result: ";
        for (std::vector<int>::const_iterator i = res.begin(); i != res.end(); ++i)
            std::cout << *i << ' ';
        cout << endl;
        std::cout << "Ground Truth: ";
        for (std::vector<int>::const_iterator i = gt.begin(); i != gt.end(); ++i)
            std::cout << *i << ' ';
        cout << endl;

        cout << "Recall: " << recall(topK, res, gt�ㄣ�ㄟ���ㄣ�ㄟ��) < precision(topK, res, gt) s << " Error Ratio: " << errorRatio(topK, res_dist, gt_dist) ion : " << precision << endl << endl;
    }

    return 0;
}
