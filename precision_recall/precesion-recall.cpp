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

float averagePrecision(int topk, vector<int> ans, vector<int> groundt){
    float old = 0, counter = 0;
    for (int i = 1; i <= topk; i++){
        float a = recall(i, ans, groundt);
        float b = precision(i, ans, groundt);
        counter = a * abs(b - old);
        old = b;
    }
    return counter;
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

    //mAP
    float meanAP = 0;

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

        meanAP = meanAP + averagePrecision(topK, res, gt);

        cout << "Recall: " << recall(topK, res, gt) << " Precision: " << precision(topK, res, gt) << " Error Ratio: " << errorRatio(topK, res_dist, gt_dist) << endl << endl;
    }
    cout << "MeanAveragePrecision: " << meanAP / entries <<endl;

    return 0;
}
