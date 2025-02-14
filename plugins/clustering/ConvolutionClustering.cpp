/**
 *
 * Copyright (C) 2019  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#include "ConvolutionClustering.h"
#include "ConvolutionClusteringSetup.h"

using namespace std;

static const char *paramHelp[] = {
    // metric
    "An existing node metric property."};

namespace tlp {

ConvolutionClustering::ConvolutionClustering(PluginContext *context)
    : DoubleAlgorithm(context), metric(nullptr) {
  addInParameter<NumericProperty *>("metric", paramHelp[0], "viewMetric", false);
}

// convolution function, build a triangular function center in 0 with a width width and a
double g(int k, double width, double amplitude) {
  double slope = amplitude / width;

  if ((k <= -width) || (k >= width))
    return 0;
  else {
    if (k < 0)
      return k * slope + amplitude; // partie croissante du signal triangulaire
    else
      return -k * slope + amplitude; // partie décroissante du signal triangulaire
  }
}
//================================================================================
int getInterval(int d, const vector<int> &ranges) {
  for (unsigned int i = 0; i < ranges.size() - 1; ++i) {
    if ((d >= ranges[i]) && (d < ranges[i + 1]))
      return i;
  }

  return ranges.size() - 2;
}
//================================================================================
void ConvolutionClustering::setParameters(int histosize, int threshold, int width) {
  this->histosize = histosize;
  this->threshold = threshold;
  this->width = width;
}
//================================================================================
void ConvolutionClustering::getParameters(int &histosize, int &threshold, int &width) {
  histosize = this->histosize;
  threshold = this->threshold;
  width = this->width;
}
//================================================================================
list<int> ConvolutionClustering::getLocalMinimum() {
  vector<double> &discretHisto = *getHistogram();
  list<int> localMinimum;
  localMinimum.push_back(0);
  double previous = discretHisto[0];
  double current = discretHisto[1];
  bool slopeSens = !(previous > current); // false descent

  for (unsigned int i = 1; i < discretHisto.size(); ++i) {
    current = discretHisto[i];
    bool newSlopeSens = !(previous > current);

    if (newSlopeSens != slopeSens) {
      // new Local minimum
      if (slopeSens == false) {
        int local = localMinimum.back();

        if (int(i) - local < width / 2) {
          localMinimum.pop_back();
          localMinimum.push_back((i + local) / 2);
        } else
          localMinimum.push_back(i);
      }

      slopeSens = newSlopeSens;
    }

    previous = current;
  }

  return localMinimum;
}
//================================================================================
void ConvolutionClustering::autoSetParameter() {

  map<double, int> histo;

  for (auto itn : graph->nodes()) {
    double tmp = metric->getNodeDoubleValue(itn);

    if (histo.find(tmp) == histo.end())
      histo[tmp] = 1;
    else
      histo[tmp] += 1;
  }

  if (histo.empty())
    return;

  //===============================================================================
  // Find good step for discretization
  // We take the minimum interval between to bar in the continue histogram
  double deltaXMin = -1;
  double deltaXMax = 0;
  double deltaSum = 0;
  auto itMap = histo.begin();
  double lastValue = (*itMap).first;
  ++itMap;

  for (; itMap != histo.end(); ++itMap) {
    double delta = itMap->first - lastValue;
    deltaSum += delta;

    if (delta > deltaXMax)
      deltaXMax = delta;
    else if (delta < deltaXMin || deltaXMin < 0)
      deltaXMin = delta;

    lastValue = (*itMap).first;
  }

  histosize = int((metric->getNodeDoubleMax() - metric->getNodeDoubleMin()) / deltaXMin);

  if (histosize > 16384)
    histosize = 16384; // histosize = histosize <? 16384;

  if (histosize < 64)
    histosize = 64; // histosize = histosize >? 64;

  //===============================================================================
  // Find good with for the convolution function
  // We take the maximum width of the biggest hole
  // width=(int)(deltaXMax*histosize/(metric->getNodeMax()-metric->getNodeMin()));
  // width=(int)(deltaXMin*histosize/(metric->getNodeMax()-metric->getNodeMin()));
  deltaSum /= histo.size();
  width = int(deltaSum * histosize / (metric->getNodeDoubleMax() - metric->getNodeDoubleMin()));
  //===============================================================================
  // Find good threshold
  // make the average of all local minimum
  vector<double> &discretHisto = *getHistogram();
  double sum = 0;
  int nbElement = 1;

  if (discretHisto.size() > 1) {
    double previous = discretHisto[0];
    double current = discretHisto[1];
    bool slopeSens = !(previous > current);

    for (unsigned int i = 1; i < discretHisto.size(); ++i) {
      double current = discretHisto[i];
      bool newSlopeSens = !(previous > current);

      if (newSlopeSens != slopeSens) {
        // new Local minimum
        nbElement++;
        sum += (current + previous) / 2;
        slopeSens = newSlopeSens;
      }

      previous = current;
    }
  }

  threshold = int(sum / nbElement);
}
//================================================================================
vector<double> *ConvolutionClustering::getHistogram() {
  // building of the histogram of values
  histogramOfValues.clear();
  double minVal = metric->getNodeDoubleMin();
  double maxMinRange = metric->getNodeDoubleMax() - minVal;

  for (auto n : graph->nodes()) {
    int tmp = int((metric->getNodeDoubleValue(n) - minVal) * histosize / maxMinRange);

    if (histogramOfValues.find(tmp) == histogramOfValues.end())
      histogramOfValues[tmp] = 1;
    else
      histogramOfValues[tmp] += 1;
  }

  // Apply the convolution on the histogram of values
  // Convolution parameter, this version work only with integer
  smoothHistogram.clear();
  smoothHistogram.resize(histosize);

  for (int pos = 0; pos < histosize; ++pos)
    smoothHistogram[pos] = 0;

  for (const auto &itMap : histogramOfValues) {
    double value = itMap.second;
    int index = itMap.first;

    for (int i = -width; i <= width; ++i) {
      if ((index + i) >= 0 && (index + i) < histosize)
        smoothHistogram[index + i] += value * g(i, width, 1);
    }
  }

  return &smoothHistogram;
}
//================================================================================
void ConvolutionClustering::getClusters(const std::vector<int> &ranges) {
  double minVal = metric->getNodeDoubleMin();
  double maxMinRange = metric->getNodeDoubleMax() - minVal;
  for (auto n : graph->nodes()) {
    int tmp = getInterval(int((metric->getNodeDoubleValue(n) - minVal) * histosize / maxMinRange),
                          ranges);
    result->setNodeValue(n, tmp);
  }
}
//================================================================================
bool ConvolutionClustering::run() {
  histosize = 128;

  if (dataSet != nullptr)
    dataSet->get("metric", metric);

  if (metric == nullptr)
    metric = graph->getDoubleProperty("viewMetric");

  autoSetParameter();
  getHistogram();

  if (ConvolutionClusteringSetup(this).exec() == QDialog::Rejected) {
    pluginProgress->setError("user cancellation");
    return false;
  }

  vector<int> ranges;
  ranges.push_back(0);
  list<int> localMinimum = getLocalMinimum();

  while (!localMinimum.empty()) {
    ranges.push_back(localMinimum.front());
    localMinimum.pop_front();
  }

  ranges.push_back(histosize);
  // Ensure that there is elements inside each intervals.
  getClusters(ranges);
  return true;
}

bool ConvolutionClustering::check(std::string &errorMsg) {
  if (dataSet != nullptr)
    dataSet->get("metric", metric);

  if (metric == nullptr)
    metric = graph->getDoubleProperty("viewMetric");

  if (metric->getNodeDoubleMax() == metric->getNodeDoubleMin()) {
    errorMsg = "All metric values are the same";
    return false;
  }

  return true;
}

PLUGIN(ConvolutionClustering)
}
