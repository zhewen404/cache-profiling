#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

template <typename T>
void plot_heatmaps(std::vector<std::vector<std::vector<T>>> heatmaps, 
    std::vector<std::vector<int>> boundary, 
    std::vector<int> heatmap_dims, 
    std::string filename)
{
    (void) boundary;
    
    // Write heatmap data to a temporary text file
    std::string temp_data_file = "heatmap_data.txt";
    std::ofstream data_file(temp_data_file);
    
    if (!data_file) {
        std::cerr << "Error opening temporary data file!" << std::endl;
        return;
    }
    
    for (size_t i = 0; i < heatmaps.size(); ++i) {
        data_file << "# Heatmap " << i + 1 << "\n";
        for (int row = 0; row < heatmap_dims[i]; ++row) {
            for (int col = 0; col < heatmap_dims[i]; ++col) {
                data_file << i << " " << row << " " << col << " " << heatmaps[i][row][col] << "\n";
            }
        }
    }
    data_file.close();
    
    // Create a Python script to plot the heatmaps using Plotly
    std::string script_file = "plot_heatmaps.py";
    std::ofstream script(script_file);
    if (!script) {
        std::cerr << "Error opening script file!" << std::endl;
        return;
    }
    
    script << "import numpy as np\n";
    script << "import pandas as pd\n";
    script << "import plotly.graph_objects as go\n";
    script << "from plotly.subplots import make_subplots\n";
    script << "df = pd.read_csv('" << temp_data_file << "', sep=' ', comment='#', header=None, names=['idx', 'row', 'col', 'value'])\n";
    script << "num_heatmaps = df['idx'].nunique()\n";
    script << "fig = make_subplots(rows=1, cols=num_heatmaps, subplot_titles=[f'Heatmap bank {i+1}' for i in range(num_heatmaps)], shared_yaxes=True)\n";
    script << "for i in range(num_heatmaps):\n";
    script << "    subset = df[df['idx'] == i].pivot(index='row', columns='col', values='value')\n";
    script << "    heatmap = go.Heatmap(z=subset.values, x=subset.columns, y=subset.index, zsmooth=False,coloraxis='coloraxis')\n";
    script << "    fig.add_trace(heatmap, row=1, col=i+1)\n";
    script << "fig.update_layout(height=600, width=600*num_heatmaps, title_text='"<<filename<< "')\n";
    // script << "fig.show()\n";
    script << "fig.update_layout(template='simple_white')\n";
    script << "fig.write_image('" << filename << "', scale=2)\n";
    script.close();
    
    // Run the Python script
    std::string command = "python3 " + script_file;
    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Error executing Python script!" << std::endl;
    }
    
    std::cout << "Heatmap saved to " << filename << std::endl;
}
