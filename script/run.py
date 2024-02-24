# this script is used to run the main program by ./bin/main <dir>
# <dir> is the directory of the input file
import os
import sys
import subprocess
from glob import glob
import numpy as np
from scipy.stats import gmean

def parse_parsec_snapshots(benchname="*"):
    dump_dir = "/home/zhewen/repo/gem5-dev-clean/gem5/m5out/m5out_fs_ruby_parsec_cachedump/anonymous_latency/version0/dump_64kB/c16/simlarge/"
    dumps = sorted(glob(dump_dir + "/" + benchname + "/[0-9]*/"))
    print(dumps)
    num_dump = len(dumps)
    print(f"parsec {benchname} contains {num_dump} snapshots.")
    return dumps

def parse_spec_snapshots(benchname="*"):
    dump_dir = f"/home/zhewen/Dropbox/result/snapshots/m5out_fs_spec2017_cachedump/dump/core4_proc4_{benchname}_seed100"
    dumps = sorted(glob(dump_dir + "/[0-9]*/"))
    print(dumps)
    num_dump = len(dumps)
    print(f"spec {benchname} contains {num_dump} snapshots.")
    return dumps

def parse_perfect_snapshots(benchname="*"):
    dump_dir = "/home/zhewen/repo/gem5-dev-clean/gem5/m5out/m5out_fs_perfect/dump/ser/"
    dumps = sorted(glob(dump_dir + "/" + benchname + "/[0-9]*/"))
    print(dumps)
    num_dump = len(dumps)
    print(f"perfect {benchname} contains {num_dump} snapshots.")
    return dumps

def launch(dir, num_banks=None, kb_per_bank=None):
    if not os.path.exists(dir):
        print("Error: directory does not exist")
        sys.exit(1)
    # launch the main program in background
    if num_banks and kb_per_bank:
        cmd = f"./bin/main {dir} {num_banks} {kb_per_bank} &"
    elif num_banks and not kb_per_bank:
        cmd = f"./bin/main {dir} {num_banks} &"
    elif not num_banks and not kb_per_bank:
        cmd = f"./bin/main {dir} &"
    else: 
        print("Error: unknown arguments")
        sys.exit(1)
    print(f"Launching {cmd}")
    os.system(cmd)


def plot_hashfunction(dumps, benchname, suitename, schemes_to_plot=None, plot_even_only=False):
    scheme_to_name = {
        "bpc": "vanila_bpc",
        "bdi": "vanila_bdi",
        "BDI": "vanila_bdi_big_end",
        "bdi-immo": "vanila_bdi_allowimmo",
        "thesaurus": "thesaurus",
        "thesaurus-immo": "thesaurus-immo",
        "shuffle-xorfold": "fbsxf",
        "thesaurus": "thesaurus",
        "bit-sampling": "bs",
        "bytemap-shuffle-xorfold": "shuffledbytemap",
        "bytemap-shuffle-xorfold-bpc": "shuffledbytemap-bpc",
        "bytemap-shuffle-xorfold-immo": "shuffledbytemap-immo",
        "ternarybytemap-shuffle-xorfold": "shuffledtbytemap",
        "lowentropy_8_4": "lowentropy_8_4",
        "lowentropy_8_16(BCD)": "lowentropy_8_16",
        "lowentropy_8_16(BCD)-immo": "lowentropy_8_16-immo",
        "sparseshuffledbytemap_8_7": "sparseshuffledbytemap_8_7",
        "sparseshuffledbytemap_8_6": "sparseshuffledbytemap_8_6",
        "sparseshuffledbytemap_8_4": "sparseshuffledbytemap_8_4",
        "sparseshuffledbytemap_4_3": "sparseshuffledbytemap_4_3",
        "sparseshuffledbytemap_4_2": "sparseshuffledbytemap_4_2",
    }
    crs_schemes = {
        "bpc": [],
        "bdi": [],
        "BDI": [],
        "bdi-immo": [],
        "shuffle-xorfold": [],
        "thesaurus": [],
        "thesaurus-immo": [],
        "bit-sampling": [],
        "bytemap-shuffle-xorfold": [],
        "bytemap-shuffle-xorfold-bpc": [],
        "bytemap-shuffle-xorfold-immo": [],
        "ternarybytemap-shuffle-xorfold": [],
        "twobytemap-shuffle-xorfold": [],
        "shuffledbytemap-shuffle-xorfold": [],
        "lowentropy_8_4": [],
        "lowentropy_8_16(BCD)": [],
        "lowentropy_8_16(BCD)-immo": [],
        "sparseshuffledbytemap_8_7": [],
        "sparseshuffledbytemap_8_6": [],
        "sparseshuffledbytemap_8_4": [],
        "sparseshuffledbytemap_4_3": [],
        "sparseshuffledbytemap_4_2": [],
    }

    ers_schemes = {
        "bpc": [],
        "bdi": [],
        "BDI": [],
        "bdi-immo": [],
        "shuffle-xorfold": [],
        "thesaurus": [],
        "thesaurus-immo": [],
        "bit-sampling": [],
        "bytemap-shuffle-xorfold": [],
        "bytemap-shuffle-xorfold-bpc": [],
        "bytemap-shuffle-xorfold-immo": [],
        "ternarybytemap-shuffle-xorfold": [],
        "twobytemap-shuffle-xorfold": [],
        "shuffledbytemap-shuffle-xorfold": [],
        "lowentropy_8_4": [],
        "lowentropy_8_16(BCD)": [],
        "lowentropy_8_16(BCD)-immo": [],
        "sparseshuffledbytemap_8_7": [],
        "sparseshuffledbytemap_8_6": [],
        "sparseshuffledbytemap_8_4": [],
        "sparseshuffledbytemap_4_3": [],
        "sparseshuffledbytemap_4_2": [],
    }

    frs_schemes = {
        "bpc": [],
        "bdi": [],
        "BDI": [],
        "bdi-immo": [],
        "shuffle-xorfold": [],
        "thesaurus": [],
        "thesaurus-immo": [],
        "bit-sampling": [],
        "bytemap-shuffle-xorfold": [],
        "bytemap-shuffle-xorfold-bpc": [],
        "bytemap-shuffle-xorfold-immo": [],
        "ternarybytemap-shuffle-xorfold": [],
        "twobytemap-shuffle-xorfold": [],
        "shuffledbytemap-shuffle-xorfold": [],
        "lowentropy_8_4": [],
        "lowentropy_8_16(BCD)": [],
        "lowentropy_8_16(BCD)-immo": [],
        "sparseshuffledbytemap_8_7": [],
        "sparseshuffledbytemap_8_6": [],
        "sparseshuffledbytemap_8_4": [],
        "sparseshuffledbytemap_4_3": [],
        "sparseshuffledbytemap_4_2": [],
    }
    intras_schemes = {
        "bpc": [],
        "bdi": [],
        "BDI": [],
        "bdi-immo": [],
        "shuffle-xorfold": [],
        "thesaurus": [],
        "thesaurus-immo": [],
        "bit-sampling": [],
        "bytemap-shuffle-xorfold": [],
        "bytemap-shuffle-xorfold-bpc": [],
        "bytemap-shuffle-xorfold-immo": [],
        "ternarybytemap-shuffle-xorfold": [],
        "twobytemap-shuffle-xorfold": [],
        "shuffledbytemap-shuffle-xorfold": [],
        "lowentropy_8_4": [],
        "lowentropy_8_16(BCD)": [],
        "lowentropy_8_16(BCD)-immo": [],
        "sparseshuffledbytemap_8_7": [],
        "sparseshuffledbytemap_8_6": [],
        "sparseshuffledbytemap_8_4": [],
        "sparseshuffledbytemap_4_3": [],
        "sparseshuffledbytemap_4_2": [],
    }

    num_points = 0

    for d in dumps:
        for scheme in schemes_to_plot:
            # Construct file name dynamically based on scheme and dump directory
            scheme_name = scheme_to_name[scheme]
            file_path = f"{d}crss-{scheme_name}.txt"
            with open(file_path, "r") as file:
                lines = file.readlines()
                assert len(lines) == 1
                l = lines[0]
                scheme_vector = list(map(float, l.split()))
                if len(scheme_vector) == 1:
                    scheme_vector = [np.nan] * 60
                num_points = len(scheme_vector)
                crs_schemes[scheme].append(scheme_vector)

            # Repeat for ERS
            file_path = f"{d}erss-{scheme_name}.txt"
            with open(file_path, "r") as file:
                lines = file.readlines()
                assert len(lines) == 1
                l = lines[0]
                scheme_vector = list(map(float, l.split()))
                if len(scheme_vector) == 1:
                    scheme_vector = [np.nan] * 60
                ers_schemes[scheme].append(scheme_vector)

            # Repeat for FRS
            file_path = f"{d}frss-{scheme_name}.txt"
            with open(file_path, "r") as file:
                lines = file.readlines()
                assert len(lines) == 1
                l = lines[0]
                scheme_vector = list(map(float, l.split()))
                if len(scheme_vector) == 1:
                    scheme_vector = [np.nan] * 60
                frs_schemes[scheme].append(scheme_vector)

            # Repeat for INTRAS
            file_path = f"{d}intras-{scheme_name}.txt"
            with open(file_path, "r") as file:
                lines = file.readlines()
                assert len(lines) == 1
                l = lines[0]
                scheme_vector = list(map(float, l.split()))
                if len(scheme_vector) == 1:
                    scheme_vector = [scheme_vector[0]] * 60
                intras_schemes[scheme].append(scheme_vector)

    # Remaining code for processing and plotting the averages...
    print(len(crs_schemes[schemes_to_plot[0]]))
    xaxis = [i+1 for i in range(num_points)]

    crs_scheme_vs_avg = {}
    ers_scheme_vs_avg = {}
    frs_scheme_vs_avg = {}
    intras_scheme_vs_avg = {}

    for scheme, data in crs_schemes.items():
        if len(data) == 0: continue
        crs_scheme_vs_avg[scheme] = [gmean(x) for x in zip(*data)]
        if scheme == "ternarybytemap-shuffle-xorfold":
            new_vec = []
            for i in range(len(crs_scheme_vs_avg[scheme])):
                if (i%2) != 0 and 2*i+1 < len(crs_scheme_vs_avg[scheme]): new_vec.append(crs_scheme_vs_avg[scheme][2*i+1])
                else: new_vec.append(np.nan)
            crs_scheme_vs_avg[scheme] = new_vec
            # print(crs_scheme_vs_avg[scheme])

        

    for scheme, data in ers_schemes.items():
        if len(data) == 0: continue
        # ers_scheme_vs_avg[scheme] = [gmean(x) for x in zip(*data)]
        ers_scheme_vs_avg[scheme] = [sum(x)/len(x) for x in zip(*data)]
        if scheme == "ternarybytemap-shuffle-xorfold":
            new_vec = []
            for i in range(len(ers_scheme_vs_avg[scheme])):
                if (i%2) != 0 and 2*i+1 < len(ers_scheme_vs_avg[scheme]): new_vec.append(ers_scheme_vs_avg[scheme][2*i+1])
                else: new_vec.append(np.nan)
            ers_scheme_vs_avg[scheme] = new_vec
            # print(ers_scheme_vs_avg[scheme])
        

    for scheme, data in frs_schemes.items():
        if len(data) == 0: continue
        # frs_scheme_vs_avg[scheme] = [gmean(x) for x in zip(*data)]
        frs_scheme_vs_avg[scheme] = [sum(x)/len(x) for x in zip(*data)]
        if scheme == "ternarybytemap-shuffle-xorfold":
            new_vec = []
            for i in range(len(frs_scheme_vs_avg[scheme])):
                if (i%2) != 0 and 2*i+1 < len(frs_scheme_vs_avg[scheme]): new_vec.append(frs_scheme_vs_avg[scheme][2*i+1])
                else: new_vec.append(np.nan)
            frs_scheme_vs_avg[scheme] = new_vec
            # print(frs_scheme_vs_avg[scheme])

    for scheme, data in intras_schemes.items():
        if len(data) == 0: continue
        intras_scheme_vs_avg[scheme] = [gmean(x) for x in zip(*data)]
        if scheme == "ternarybytemap-shuffle-xorfold":
            new_vec = []
            for i in range(len(intras_scheme_vs_avg[scheme])):
                if (i%2) != 0 and 2*i+1 < len(intras_scheme_vs_avg[scheme]): new_vec.append(intras_scheme_vs_avg[scheme][2*i+1])
                else: new_vec.append(np.nan)
            intras_scheme_vs_avg[scheme] = new_vec
            # print(intras_scheme_vs_avg[scheme])

    if plot_even_only:
        xaxis = xaxis[1::2]
        for scheme in crs_scheme_vs_avg:
            crs_scheme_vs_avg[scheme] = crs_scheme_vs_avg[scheme][1::2]

        for scheme in ers_scheme_vs_avg:
            ers_scheme_vs_avg[scheme] = ers_scheme_vs_avg[scheme][1::2]

        for scheme in frs_scheme_vs_avg:
            frs_scheme_vs_avg[scheme] = frs_scheme_vs_avg[scheme][1::2]

        for scheme in intras_scheme_vs_avg:
            intras_scheme_vs_avg[scheme] = intras_scheme_vs_avg[scheme][1::2]

    import plotly.graph_objects as go
    import plotly.subplots as sp
    import pandas as pd
    import plotly.io as pio
    pio.kaleido.scope.mathjax = None

    fig = sp.make_subplots(rows=1, cols=4, 
                        #    subplot_titles=("Compression Ratio", "Entropy Reduction", "False Positive Rate"),
                           shared_yaxes=False,
                           horizontal_spacing=0.05,
                           vertical_spacing=0,)

    color_sequence = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', 
                      '#8c564b', '#e377c2', '#7f7f7f', '#bcbd22', '#17becf']
    i = 0
    for scheme, data in crs_scheme_vs_avg.items():
        fig.add_trace(
            go.Scatter(x=xaxis,
                    y=data,
                    mode='markers+lines',
                    name=scheme,
                    legendgroup=scheme,
                    line=dict(color=color_sequence[i]), 
                    ),

            row=1, col=1)
        i += 1
    fig.update_yaxes(title="inter comp. ratio", row=1, col=1)
    
    i=0
    for scheme, data in ers_scheme_vs_avg.items():
        fig.add_trace(
            go.Scatter(x=xaxis,
                    y=data,
                    mode='markers+lines',
                    name=scheme,
                    legendgroup=scheme,
                    line=dict(color=color_sequence[i]), 
                    showlegend=False),
            row=1, col=2)
        i += 1
    fig.update_yaxes(title="entropy reduction", row=1, col=2, title_standoff = 0)
    
    i=0
    for scheme, data in frs_scheme_vs_avg.items():
        fig.add_trace(
            go.Scatter(x=xaxis,
                    y=data,
                    mode='markers+lines',
                    name=scheme,
                    legendgroup=scheme,
                    line=dict(color=color_sequence[i]), 
                    showlegend=False),
            row=1, col=3)
        i += 1
    fig.update_yaxes(title="false positive rate", row=1, col=3, title_standoff = 0)

    i=0
    for scheme, data in intras_scheme_vs_avg.items():
        fig.add_trace(
            go.Scatter(x=xaxis,
                    y=data,
                    mode='markers+lines',
                    name=scheme,
                    legendgroup=scheme,
                    line=dict(color=color_sequence[i]), 
                    showlegend=False),
            row=1, col=4)
        i += 1
    fig.update_yaxes(title="intra comp. ratio", row=1, col=4, title_standoff = 0)


    dpi = 300
    w=3.3115
    h=1.3
    fig.update_yaxes(ticks="outside",
                 ticklen=0,  # adjust length of the tick = distance from axis
                 )
    # make the legend horizontal at the bottom
    fig.update_layout(legend=dict(
        orientation="h",
        yanchor="bottom",
        y=-0.4,
        xanchor="right",
        x=1
    ))
    fig.update_layout(
        template="plotly_white",
        width=2*w*dpi,
        height=h*dpi,
        paper_bgcolor='rgba(0,0,0,0)',
        title=None,
        xaxis_title=None,
        margin=dict(l=5, r=5, t=5, b=5),
        font=dict(
            family="ubuntu",
            size=22,
        ),
        bargap=0.10,
        bargroupgap=0.0,
    )

    if plot_even_only:
        fig.write_image(f"img/{suitename}/hash-{benchname}-even.pdf")
    else:
        fig.write_image(f"img/{suitename}/hash-{benchname}.pdf")

def plot_profiling(dumps, benchname, suitename, stats_to_plot=None):
    stat_to_name = {
        "entropy byte position": "entropy_byte_position",
        "entropy byte position<br>after xor rand bank": "entropy_byte_position_afterxor_randbank",
        "entropy byte position<br>after xor bytemap": "entropy_byte_position_afterxor12_bytemap",
        "entropy byte position<br>after xor thesaurus": "entropy_byte_position_afterxor12_thesaurus",
        "entropy byte position<br>after xor lowentropy_8_16(BCD)": "entropy_byte_position_afterxor12_lowentropy_8_16",
        "entropy byte position<br>after xor lowentropy_8_16(BCD) only xored": "entropy_byte_position_afterxor12_lowentropy_8_16-onlyxored",
    }
    stat_arr = {
        "entropy byte position": [],
        "entropy byte position<br>after xor rand bank": [],
        "entropy byte position<br>after xor bytemap": [],
        "entropy byte position<br>after xor thesaurus": [],
        "entropy byte position<br>after xor lowentropy_8_16(BCD)": [],
        "entropy byte position<br>after xor lowentropy_8_16(BCD) only xored": [],
    }

    for d in dumps:
        for stat in stats_to_plot:
            # Construct file name dynamically based on scheme and dump directory
            stat_name = stat_to_name[stat]
            file_path = f"{d}profiling-{stat_name}.txt"
            with open(file_path, "r") as file:
                lines = file.readlines()
                assert len(lines) == 1
                l = lines[0]
                stat_vector = list(map(float, l.split()))
            
            stat_arr[stat].append(stat_vector)
    # #print stat_arr
    # for stat, data in stat_arr.items():
    #     print(stat, len(data))
    #     print(data[0])

    # Remaining code for processing and plotting the averages...
    print(len(stat_arr[stats_to_plot[0]]))

    stats_avg = {}

    for stat, data in stat_arr.items():
        if len(data) == 0: continue
        # stats_avg[stat] = [gmean(x) for x in zip(*data)]
        stats_avg[stat] = [sum(x)/len(x) for x in zip(*data)]
    
    # # print stats_avg
    # for stat, data in stats_avg.items():
    #     print(stat, len(data))
    #     print(data)

    import plotly.graph_objects as go
    import plotly.subplots as sp
    import pandas as pd
    import plotly.io as pio
    pio.kaleido.scope.mathjax = None

    dpi = 300
    w=3.3115
    h=1.3
    fig = sp.make_subplots(rows=1, cols=len(stats_to_plot), 
                        #    subplot_titles=("Compression Ratio", "Entropy Reduction", "False Positive Rate"),
                           shared_yaxes=False,
                           horizontal_spacing=0.1,
                           vertical_spacing=0,)

    color_sequence = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', 
                      '#8c564b', '#e377c2', '#7f7f7f', '#bcbd22', '#17becf']
    
    num = 1
    for stat, data in stats_avg.items():
        fig.add_trace(
            go.Scatter(x=list(range(len(data))),
                    y=data,
                    mode='markers+lines',
                    # name=stat,
                    # legendgroup=num,
                    line=dict(color=color_sequence[0]), 
                    showlegend=False,
                    ),

            row=1, col=num)
        fig.update_yaxes(title=stat, row=1, col=num)

        if "entropy byte position" in stat:
            # plot word boundary as vertical lines
            for i in range(0, 64, 8):
                fig.add_shape(
                    # Line Vertical
                    dict(
                        type="line",
                        x0=i,
                        y0=0,
                        x1=i,
                        y1=8,
                        line=dict(
                            color="Black",
                            width=1,
                            dash="dot",
                        )
                    ),
                    row=1, col=num
                )
        
            # reshape data into 8x8 matrix
            data = np.array(data).reshape(8, 8)
            # take mean of each column
            data = np.mean(data, axis=0)
            # reverse the order of the data
            data = data[::-1]
            fig_new = go.Figure()
            # plot a vector heatmap with min=0 and max=8
            # annotate the heatmap with the data
            fig_new.add_trace(
                go.Heatmap(
                    z=[data],
                    text=[[7,6,5,4,3,2,1,0]],
                    texttemplate="%{text}",
                    textfont={"size":20},
                    colorscale='RdBu_r',
                    zmin=0,
                    zmax=8,
                    showscale=False,
                    x0=0,
                    dx=1,
                    y0=0,
                    dy=1,
                    hoverinfo='z',
                )
            )
            # hide y-axis
            fig_new.update_yaxes(visible=False)
            # hide x-axis
            fig_new.update_xaxes(visible=False)
            
            fig_new.update_layout(
                template="plotly_white",
                width=0.5*w*dpi,
                height=0.15*h*dpi,
                paper_bgcolor='rgba(0,0,0,0)',
                title=None,
                xaxis_title=None,
                margin=dict(l=5, r=5, t=5, b=5),
                font=dict(
                    family="ubuntu",
                    size=22,
                ),
                bargap=0.10,
                bargroupgap=0.0,
            )
            fig_new.write_image(f"img/{suitename}/profiling-{stat_to_name[stat]}-{benchname}-word-byte-entropy-heatmap.pdf")

            fig_cs = go.Figure()
            # plot a vector heatmap with min=0 and max=8
            # annotate the heatmap with the data
            fig_cs.add_trace(
                go.Heatmap(
                    z=[np.linspace(0, 8, 80)],
                    # text=[[7,6,5,4,3,2,1,0]],
                    # texttemplate="%{text}",
                    # textfont={"size":20},
                    colorscale='RdBu_r',
                    zmin=0,
                    zmax=8,
                    showscale=False,
                    x0=0,
                    dx=1,
                    y0=0,
                    dy=1,
                    hoverinfo='z',
                )
            )
            # hide y-axis
            fig_cs.update_yaxes(visible=False)
            # show x-axis with sparse ticks
            fig_cs.update_xaxes(
                tickvals=[0, 39, 79],
                ticktext=["0", "4", "8"],
                ticklen=0,
            )
            
            fig_cs.update_layout(
                template="plotly_white",
                width=0.5*w*dpi,
                height=0.15*h*dpi,
                paper_bgcolor='rgba(0,0,0,0)',
                title=None,
                xaxis_title=None,
                margin=dict(l=5, r=5, t=5, b=20),
                font=dict(
                    family="ubuntu",
                    size=16,
                ),
                bargap=0.10,
                bargroupgap=0.0,
            )
            fig_cs.write_image(f"img/{suitename}/profiling-{stat_to_name[stat]}-{benchname}-word-byte-entropy-heatmap-cs.pdf")
        
        num += 1
    


    fig.update_yaxes(ticks="outside",
                 ticklen=0,  # adjust length of the tick = distance from axis
                 )
    # make the legend horizontal at the bottom
    fig.update_layout(legend=dict(
        orientation="h",
        yanchor="bottom",
        y=-0.4,
        xanchor="right",
        x=1
    ))
    fig.update_layout(
        template="plotly_white",
        width=w*dpi,
        height=h*dpi,
        paper_bgcolor='rgba(0,0,0,0)',
        title=None,
        xaxis_title=None,
        margin=dict(l=5, r=5, t=5, b=5),
        font=dict(
            family="ubuntu",
            size=22,
        ),
        bargap=0.10,
        bargroupgap=0.0,
    )
    fig.write_image(f"img/{suitename}/profiling-{benchname}.pdf")


if __name__ == "__main__":
    benchname = "*"
    suitename = "allsuite"
    launch_flag = False
    # plot="profiling"
    plot="hash"

    num_banks = None
    kb_per_bank = None
    if suitename == "parsec":
        dumps = parse_parsec_snapshots(benchname)
        num_banks = 16
        kb_per_bank = 64
    elif suitename == "spec":
        dumps = parse_spec_snapshots(benchname)
    elif suitename == "perfect":
        dumps = parse_perfect_snapshots(benchname)
        num_banks = 1
    elif suitename == "allsuite":
        dumps = parse_parsec_snapshots("*") + parse_spec_snapshots("*") + parse_perfect_snapshots("*")
    else:
        print("Error: unknown suitename")
        sys.exit(1)

    if launch_flag:
        assert suitename != "allsuite"
        for d in dumps:
            launch(d, num_banks, kb_per_bank)
        print(len(dumps))

    if not launch_flag:
        if benchname == "*" or suitename == "allsuite": benchname = "all"
        if plot == "profiling":
            plot_profiling(dumps, benchname, suitename,
                           stats_to_plot=[
                               "entropy byte position",
                            #    "entropy byte position<br>after xor rand bank",
                            #    "entropy byte position<br>after xor bytemap",
                            #    "entropy byte position<br>after xor thesaurus",
                               "entropy byte position<br>after xor lowentropy_8_16(BCD)",
                               "entropy byte position<br>after xor lowentropy_8_16(BCD) only xored",
                           ])
        elif plot == "hash":
            plot_hashfunction(dumps, benchname, suitename, 
                    schemes_to_plot=[
                        "bpc",
                        "bdi",
                        # "BDI",
                        # "bdi-immo",
                        # "thesaurus",
                        # "thesaurus-immo",
                        "bytemap-shuffle-xorfold", 
                        "bytemap-shuffle-xorfold-bpc", 
                        # "bytemap-shuffle-xorfold-immo", 
                        # "sparseshuffledbytemap_8_7",
                        # "sparseshuffledbytemap_8_6",
                        # "sparseshuffledbytemap_8_4",
                        # "sparseshuffledbytemap_4_3",
                        # "sparseshuffledbytemap_4_2",
                        # "ternarybytemap-shuffle-xorfold",
                        # "lowentropy_8_4",
                        # "lowentropy_8_16(BCD)",
                        # "lowentropy_8_16(BCD)-immo",
                        ])
