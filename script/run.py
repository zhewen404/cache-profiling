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


def print_avg(dumps, benchname, suitename, schemes_to_plot=None, plot_even_only=False):
    scheme_to_name = {
        "bdi": "vanila_bdi",
        "BDI": "vanila_bdi_big_end",
        "thesaurus": "thesaurus",
        "shuffle-xorfold": "fbsxf",
        "thesaurus": "thesaurus",
        "bit-sampling": "bs",
        "bytemap-shuffle-xorfold": "shuffledbytemap",
        "ternarybytemap-shuffle-xorfold": "shuffledtbytemap",
        "lowentropy_8_4": "lowentropy_8_4",
        "lowentropy_8_16(BCD)": "lowentropy_8_16",
        "sparseshuffledbytemap_8_6": "sparseshuffledbytemap_8_6",
        "sparseshuffledbytemap_8_4": "sparseshuffledbytemap_8_4",
        "sparseshuffledbytemap_4_3": "sparseshuffledbytemap_4_3",
        "sparseshuffledbytemap_4_2": "sparseshuffledbytemap_4_2",
    }
    crs_schemes = {
        "bdi": [],
        "BDI": [],
        "shuffle-xorfold": [],
        "thesaurus": [],
        "bit-sampling": [],
        "bytemap-shuffle-xorfold": [],
        "ternarybytemap-shuffle-xorfold": [],
        "twobytemap-shuffle-xorfold": [],
        "shuffledbytemap-shuffle-xorfold": [],
        "lowentropy_8_4": [],
        "lowentropy_8_16(BCD)": [],
        "sparseshuffledbytemap_8_6": [],
        "sparseshuffledbytemap_8_4": [],
        "sparseshuffledbytemap_4_3": [],
        "sparseshuffledbytemap_4_2": [],
    }

    ers_schemes = {
        "bdi": [],
        "BDI": [],
        "shuffle-xorfold": [],
        "thesaurus": [],
        "bit-sampling": [],
        "bytemap-shuffle-xorfold": [],
        "ternarybytemap-shuffle-xorfold": [],
        "twobytemap-shuffle-xorfold": [],
        "shuffledbytemap-shuffle-xorfold": [],
        "lowentropy_8_4": [],
        "lowentropy_8_16(BCD)": [],
        "sparseshuffledbytemap_8_6": [],
        "sparseshuffledbytemap_8_4": [],
        "sparseshuffledbytemap_4_3": [],
        "sparseshuffledbytemap_4_2": [],
    }

    frs_schemes = {
        "bdi": [],
        "BDI": [],
        "shuffle-xorfold": [],
        "thesaurus": [],
        "bit-sampling": [],
        "bytemap-shuffle-xorfold": [],
        "ternarybytemap-shuffle-xorfold": [],
        "twobytemap-shuffle-xorfold": [],
        "shuffledbytemap-shuffle-xorfold": [],
        "lowentropy_8_4": [],
        "lowentropy_8_16(BCD)": [],
        "sparseshuffledbytemap_8_6": [],
        "sparseshuffledbytemap_8_4": [],
        "sparseshuffledbytemap_4_3": [],
        "sparseshuffledbytemap_4_2": [],
    }
    intras_schemes = {
        "bdi": [],
        "BDI": [],
        "shuffle-xorfold": [],
        "thesaurus": [],
        "bit-sampling": [],
        "bytemap-shuffle-xorfold": [],
        "ternarybytemap-shuffle-xorfold": [],
        "twobytemap-shuffle-xorfold": [],
        "shuffledbytemap-shuffle-xorfold": [],
        "lowentropy_8_4": [],
        "lowentropy_8_16(BCD)": [],
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
        fig.write_image(f"img/{suitename}/{benchname}-even.pdf")
    else:
        fig.write_image(f"img/{suitename}/{benchname}.pdf")

if __name__ == "__main__":
    benchname = "*"
    suitename = "allsuite"
    launch_flag = False

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
        print_avg(dumps, benchname, suitename, 
                schemes_to_plot=[
                    "bdi",
                    # "BDI",
                    "thesaurus",
                    "bytemap-shuffle-xorfold", 
                    "sparseshuffledbytemap_8_6",
                    # "sparseshuffledbytemap_8_4",
                    # "sparseshuffledbytemap_4_3",
                    # "sparseshuffledbytemap_4_2",
                    # "ternarybytemap-shuffle-xorfold",
                    #   "lowentropy_8_4",
                    "lowentropy_8_16(BCD)",
                    ])
