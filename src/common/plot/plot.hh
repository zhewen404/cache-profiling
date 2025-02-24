#ifndef COMMON_PLOT_PLOT_HH
#define COMMON_PLOT_PLOT_HH

#include <sciplot/sciplot.hpp>
#include <vector>
#include <string>
#include <bits/stdc++.h> 
using namespace sciplot;

template <typename T>
void plot_2d(Plot2D* plot, 
    std::vector<T> xvector, std::vector<T> yvector, 
    std::string legend, std::string xlabel, std::string ylabel)
{
    // Create a Plot object

    // Set the x and y labels
    plot->xlabel(xlabel);
    plot->ylabel(ylabel);

    // Set the x and y ranges
    T min_x = *min_element(xvector.begin(), xvector.end());
    T max_x = *max_element(xvector.begin(), xvector.end());
    T min_y = *min_element(yvector.begin(), yvector.end());
    T max_y = *max_element(yvector.begin(), yvector.end());
    plot->xrange(min_x, max_x);
    plot->yrange(min_y, max_y);

    // Set the legend to be on the bottom along the horizontal
    plot->legend()
        .atOutsideBottom()
        .displayHorizontal()
        .displayExpandWidthBy(2);

    plot->drawCurve(xvector, yvector).label(legend);
    // return plot;
}

template <typename T>
void plot_2dxn(Plot2D* plot, 
    std::vector<T> xvector, std::vector<std::vector<T>> yvectors, 
    std::vector<std::string> legends, 
    std::string xlabel, std::string ylabel)
{
    // Create a Plot object

    // Set the x and y labels
    plot->xlabel(xlabel);
    plot->ylabel(ylabel);

    // Set the x and y ranges
    T min_x = *min_element(xvector.begin(), xvector.end());
    T max_x = *max_element(xvector.begin(), xvector.end());
    T min_y = std::numeric_limits<T>::max();
    T max_y = std::numeric_limits<T>::min();
    for (unsigned i = 0; i < yvectors.size(); i++)
    {
        T min_y_ = *min_element(yvectors[i].begin(), yvectors[i].end());
        T max_y_ = *max_element(yvectors[i].begin(), yvectors[i].end());
        min_y = std::min(min_y, min_y_);
        max_y = std::max(max_y, max_y_);
    }
    // printf("min_y: %f, max_y: %f\n", min_y, max_y);
    // printf("min_x: %f, max_x: %f\n", min_x, max_x);
    plot->xrange(min_x, max_x);
    plot->yrange(min_y, max_y);

    // Set the legend to be on the bottom along the horizontal
    plot->legend()
        .atOutsideBottom()
        .displayHorizontal()
        .displayExpandWidthBy(2);

    for (unsigned i = 0; i < yvectors.size(); i++)
    {
        // print yvectors[i]
        // printf("yvectors[%d]: ", i);
        // for (unsigned j = 0; j < yvectors[i].size(); j++){
        //     printf("%f ", yvectors[i][j]);
        // }
        // printf("\n now ploting\n");
        plot->drawCurve(xvector, yvectors[i]).label(legends[i]);
    }
}

template <typename T>
void plot2dx1(std::vector<T> xvector, std::vector<T> yvector, 
    std::string legend, std::string xlabel, std::string ylabel, 
    std::string filename)
{
    // Create a Plot object
    Plot2D plot1;
    plot_2d<T>(&plot1, xvector, yvector, legend, xlabel, ylabel);


    // Create figure to hold plot
    Figure fig = {{plot1}};
    // Create canvas to hold figure
    Canvas canvas = {{fig}};

    // Show the plot in a pop-up window
    // canvas.show();

    // Save the plot to a PDF file
    canvas.save(filename);
}

template <typename T>
void plot2dx2(std::vector<T> xvector, std::vector<std::vector<T>> yvectors, 
    std::string legend, std::vector<std::string> xlabels, std::vector<std::string> ylabels, 
    std::string filename)
{
    // Create a Plot object
    // std::vector<T> xvector = xvectors[i];
    std::vector<T> yvector = yvectors[0];
    std::string xlabel = xlabels[0];
    std::string ylabel = ylabels[0];
    Plot2D plot0;
    plot0.fontName("ubuntu");
    plot_2d<T>(&plot0, xvector, yvector, legend, xlabel, ylabel);

    // std::vector<T> xvector = xvectors[i];
    yvector = yvectors[1];
    xlabel = xlabels[1];
    ylabel = ylabels[1];
    Plot2D plot1;
    plot1.fontName("ubuntu");
    plot_2d<T>(&plot1, xvector, yvector, legend, xlabel, ylabel);

    // Create figure to hold plot
    Figure fig = {{plot0, plot1}};
    // Create canvas to hold figure
    Canvas canvas = {{fig}};

    // Show the plot in a pop-up window
    // canvas.show();

    // Save the plot to a PDF file
    canvas.save(filename);
}

template <typename T>
void plot2dx1xn(std::vector<T> xvector, std::vector<std::vector<T>> yvectors, 
    std::vector<std::string> legends, std::string xlabel, std::string ylabel, 
    std::string filename)
{
    Plot2D plot0;
    plot_2dxn<T>(&plot0, xvector, yvectors, legends, xlabel, ylabel);


    // Create figure to hold plot
    Figure fig = {{plot0}};
    // Create canvas to hold figure
    Canvas canvas = {{fig}};

    // Show the plot in a pop-up window
    // canvas.show();

    // Save the plot to a PDF file
    canvas.save(filename);
}


template <typename T>
void plot2dx2xn(std::vector<T> xvector, std::vector<std::vector<std::vector<T>>> yvectorss, 
    std::vector<std::string> legends, std::vector<std::string> xlabels, std::vector<std::string> ylabels, 
    std::string filename)
{
    // printf("plot2dx2xn\n");
    // Create a Plot object
    // std::vector<T> xvector = xvectors[i];
    std::vector<std::vector<T>> yvectors = yvectorss[0];

    // // print yvectors
    // for (unsigned i = 0; i < yvectors.size(); i++){
    //     for (unsigned j = 0; j < yvectors[i].size(); j++){
    //         printf("%f ", yvectors[i][j]);
    //     }
    //     printf("\n");
    // }
    // std::string legend = legends[0];
    std::string xlabel = xlabels[0];
    std::string ylabel = ylabels[0];
    Plot2D plot0;
    plot_2dxn<T>(&plot0, xvector, yvectors, legends, xlabel, ylabel);

    // std::vector<T> xvector = xvectors[i];
    yvectors = yvectorss[1];
    // legend = legends[1];
    xlabel = xlabels[1];
    ylabel = ylabels[1];
    Plot2D plot1;
    plot_2dxn<T>(&plot1, xvector, yvectors, legends, xlabel, ylabel);

    // Create figure to hold plot
    Figure fig = {{plot0, plot1}};
    // Create canvas to hold figure
    Canvas canvas = {{fig}};

    // Show the plot in a pop-up window
    // canvas.show();

    // Save the plot to a PDF file
    canvas.save(filename);
}


template <typename T>
void plot2dx3xn(std::vector<T> xvector, std::vector<std::vector<std::vector<T>>> yvectorss, 
    std::vector<std::string> legends, std::vector<std::string> xlabels, std::vector<std::string> ylabels, 
    std::string filename)
{
    // printf("plot2dx3xn\n");
    // Create a Plot object
    // std::vector<T> xvector = xvectors[i];
    std::vector<std::vector<T>> yvectors = yvectorss[0];

    // // print yvectors
    // for (unsigned i = 0; i < yvectors.size(); i++){
    //     for (unsigned j = 0; j < yvectors[i].size(); j++){
    //         printf("%f ", yvectors[i][j]);
    //     }
    //     printf("\n");
    // }
    // std::string legend = legends[0];
    std::string xlabel = xlabels[0];
    std::string ylabel = ylabels[0];
    Plot2D plot0;
    plot_2dxn<T>(&plot0, xvector, yvectors, legends, xlabel, ylabel);

    yvectors = yvectorss[1];
    xlabel = xlabels[1];
    ylabel = ylabels[1];
    Plot2D plot1;
    plot_2dxn<T>(&plot1, xvector, yvectors, legends, xlabel, ylabel);

    yvectors = yvectorss[2];
    xlabel = xlabels[2];
    ylabel = ylabels[2];
    Plot2D plot2;
    plot_2dxn<T>(&plot2, xvector, yvectors, legends, xlabel, ylabel);

    // Create figure to hold plot
    Figure fig = {{plot0, plot1, plot2}};
    // Create canvas to hold figure
    Canvas canvas = {{fig}};

    // Show the plot in a pop-up window
    // canvas.show();

    // Save the plot to a PDF file
    canvas.save(filename);
}


#endif