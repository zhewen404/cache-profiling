# this script is used to run the main program by ./bin/main <dir>
# <dir> is the directory of the input file
import os
import sys
import subprocess
from glob import glob
import numpy as np
from scipy.stats import gmean
import argparse
from utils import *
import plotly.graph_objects as go
import plotly.subplots as sp
import pandas as pd
import plotly.io as pio
pio.kaleido.scope.mathjax = None


def parse_parsec_snapshots(benchname="*"):
    home = os.path.expanduser('~')
    # dump_dir = f"{home}/Dropbox/result/snapshots/m5out_fs_ruby_parsec_cachedump/anonymous_latency/version0/dump_64kB/c16/simlarge/"
    dump_dir = f"{home}/Dropbox/result/snapshots_new/m5out_fs_ruby_parsec_cachedump/anonymous_latency/version0/dump_64kB/c4/simlarge/"
    dumps = sorted(glob(dump_dir + "/" + benchname + "/[0-9]*/"))
    # print(dumps)
    num_dump = len(dumps)
    print(f"parsec {benchname} contains {num_dump} snapshots.")
    return dumps

def parse_spec_snapshots(benchname="*"):
    home = os.path.expanduser('~')
    # dump_dir = f"{home}/Dropbox/result/snapshots/m5out_fs_spec2017_cachedump/dump/core4_proc4_{benchname}_seed100"
    dump_dir = f"{home}/Dropbox/result/snapshots_new/m5out_fs_spec2017_cachedump/dump/core4_proc4_{benchname}_seed100"
    dumps = sorted(glob(dump_dir + "/[0-9]*/"))
    # print(dumps)
    num_dump = len(dumps)
    print(f"spec {benchname} contains {num_dump} snapshots.")
    return dumps

def parse_perfect_snapshots(benchname="*"):
    home = os.path.expanduser('~')
    # dump_dir = f"{home}/Dropbox/result/snapshots/m5out_fs_perfect/dump/ser/"
    dump_dir = f"{home}/Dropbox/result/snapshots_new/m5out_fs_perfect/dump/omp/"
    dumps = sorted(glob(dump_dir + "/" + benchname + "/[0-9]*/"))
    # print(dumps)
    num_dump = len(dumps)
    print(f"perfect {benchname} contains {num_dump} snapshots.")
    return dumps

def get_geomean_cr(dumps, vertical_slice_index, scheme, hashSchemeMaps):
    res_arr = [[],[]]

    for d in dumps:
        # Construct file name dynamically based on scheme and dump directory
        scheme_name = hashSchemeMaps.scheme_to_name[scheme]
        file_path = f"{d}crss-{scheme_name}.txt"
        with open(file_path, "r") as file:
            lines = file.readlines()
            assert len(lines) == 3, print(f"len(lines) = {len(lines)}")
            # print(lines)
            # crs_arr = [hashSchemeMaps.crs_schemes, hashSchemeMaps.crs_max_schemes, hashSchemeMaps.crs_min_schemes]
            i = 0
            l = lines[i]
            scheme_vector = list(map(float, l.split()))
            num_points = len(scheme_vector)
            if len(scheme_vector) == 0:
                num = np.nan
            elif len(scheme_vector) == 1:
                if "epc_word_labeling" in scheme_name or "hycomp_word_labeling" in scheme_name or "semantic_word_labeling" in scheme_name or \
                    "averagebytemsb_word_labeling_24" in scheme_name:
                    num = scheme_vector[0]
                    scheme_vector = [np.nan] * 32
                    scheme_vector[23] = num
                elif "strong_word_labeling" in scheme_name or "density_word_labeling" in scheme_name or \
                    "averagebytemsb_word_labeling_16" in scheme_name:
                    num = scheme_vector[0]
                    scheme_vector = [np.nan] * 32
                    scheme_vector[15] = num
                elif "averagebytemsb_word_labeling_32" in scheme_name:
                    num = scheme_vector[0]
                    scheme_vector = [np.nan] * 32
                    scheme_vector[31] = num
                elif scheme_vector[0] != 0:
                    num = scheme_vector[0]
                    scheme_vector = [num] * 32
                else:
                    num = 1
                    scheme_vector = [np.nan] * 32
            else: 
                num = scheme_vector[vertical_slice_index]
            # print(num)
            res_arr[0].append(num)


        # Construct file name dynamically based on scheme and dump directory
        scheme_name = hashSchemeMaps.scheme_to_name[scheme]
        file_path = f"{d}intras-{scheme_name}.txt"
        with open(file_path, "r") as file:
            lines = file.readlines()
            assert len(lines) == 3, print(f"len(lines) = {len(lines)}")
            # print(lines)
            i = 0
            l = lines[i]
            scheme_vector = list(map(float, l.split()))
            num_points = len(scheme_vector)
            if len(scheme_vector) == 0:
                num = np.nan
            elif len(scheme_vector) == 1:
                if "epc_word_labeling" in scheme_name or "hycomp_word_labeling" in scheme_name or "semantic_word_labeling" in scheme_name or \
                    "averagebytemsb_word_labeling_24" in scheme_name:
                    num = scheme_vector[0]
                    scheme_vector = [np.nan] * 32
                    scheme_vector[23] = num
                elif "strong_word_labeling" in scheme_name or "density_word_labeling" in scheme_name or \
                    "averagebytemsb_word_labeling_16" in scheme_name:
                    num = scheme_vector[0]
                    scheme_vector = [np.nan] * 32
                    scheme_vector[15] = num
                elif "averagebytemsb_word_labeling_32" in scheme_name:
                    num = scheme_vector[0]
                    scheme_vector = [np.nan] * 32
                    scheme_vector[31] = num
                elif scheme_vector[0] != 0:
                    num = scheme_vector[0]
                    scheme_vector = [num] * 32
                else:
                    num = 1
                    scheme_vector = [np.nan] * 32
            else: 
                num = scheme_vector[vertical_slice_index]
            # print(num)
            res_arr[1].append(num)
        
    product_arr = [a*b for a,b in zip(res_arr[0], res_arr[1])]
    res_arr.append(product_arr)

    inter_ideal = 1 if scheme=="BDI (no XOR)" else 2
    productIdeal_arr = [inter_ideal*b for b in res_arr[1]]
    res_arr.append(productIdeal_arr)

    ret_arr = []
    for arr in res_arr:
        geomean = float(gmean(arr))
        ret_arr.append(geomean)
    
    return ret_arr, ["inter", "intra", "total", "totalIdeal"]


def format_fig(fig, onlygeomean, fignum):
    dpi = 300
    w=3.3115
    if onlygeomean: w = w/2
    if fignum == 2: w = w*2
    h=1.3
    fig.update_yaxes(ticks="outside",
                 ticklen=0,  # adjust length of the tick = distance from axis
                 )
    # make the legend horizontal at the bottom
    # if plot_final: ew = 200
    # else: ew=None
    if fignum == 4:
            h=1.1
            fig.update_layout(
            legend=dict(
            # entrywidth=ew,
            # entrywidthmode='fraction',
            orientation="h",
            yanchor="middle",
            y=0.5,
            xanchor="left",
            x=1,
            font=dict(
                size=20,
            )
        ))
    else:
        fig.update_layout(
            legend=dict(
            # entrywidth=ew,
            # entrywidthmode='fraction',
            orientation="h",
            yanchor="bottom",
            y=1.02,
            xanchor="left",
            x=0.05,
            font=dict(
                size=22,
            )
        ))

    # if plot_final: width = w
    # else: width = 2*w

    fig.update_layout(
        template="plotly_white",
        width=w*dpi,
        height=h*dpi,
        paper_bgcolor='rgba(0,0,0,0)',
        title=None,
        xaxis_title=None,
        yaxis_title="Compression Ratio",
        margin=dict(l=2, r=2, t=2, b=2),
        font=dict(
            family="ubuntu",
            size=22,
        ),
        bargap=0.10,
        bargroupgap=0.0,
    )
    fig.update_traces(marker_line_width = 0,
                  selector=dict(type="bar"),
                  marker_pattern_shape=None,)
    
    fig.update_xaxes(
        # tickangle=90,
        type='multicategory',
        # tickvals=np.arange(len(new)),
        # ticktext=new,
        tickson="boundaries",
        ticklen=0,
        linewidth=0,
        tickfont=dict(size=21),
        showdividers=False,
        # dividerlength=1,
    )
    fig.update_yaxes(
        gridwidth=0,
    )

    return fig

if __name__ == "__main__":
    # add argument to specify suitename, launch_flag, and plot
    parser = argparse.ArgumentParser(description='run and plot script')
    parser.add_argument('--suitename', 
                        help='suitename',
                        choices=["parsec", "spec", "perfect", "allsuite", "pp"])
    parser.add_argument('--fig',
                        help='set name',
                        type = int,
                        choices=[2, 4, 12],
                        default=2)
    parser.add_argument('--onlygeomean',
                        help='plot only the geomean',
                        action='store_true')
    parser.add_argument('--norm',
                        help='plot the final results normalized',
                        action='store_true')
    parser.add_argument('--plot_final',
                        help='plot the final results',
                        action='store_true')
    parser.add_argument('--bit',
                        type=int,
                        default=7,
                        help='bit to plot',)
    args = parser.parse_args()

    # benchname = args.benchname
    suitename = args.suitename
    assert args.bit >= 1 and args.bit <= 32, print("Error: bit should be between 1 and 32")
    # plot="profiling"

    num_banks = None
    kb_per_bank = None

    parsec_yaml = yaml_load("script/parsec_default.yml")
    parsec_bm = [dict_['name'] for dict_ in parsec_yaml.values()]
    parsec_short = [dict_['short'] for dict_ in parsec_yaml.values()]
    parsec_bm = parsec_bm[0:-1]
    parsec_short = parsec_short[0:-1]

    perfect_yaml = yaml_load("script/perfect_default.yml")
    perfect_bm = [dict_['name'] for dict_ in perfect_yaml.values()]
    perfect_short = [dict_['short'] for dict_ in perfect_yaml.values()]

    spec_bm = ["run1", "run2", "run3", "run4", "run5", "run6", "run7", "run8", "run9", "run10", "run11"]
    spec_short = ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"]

    if suitename == "parsec":
        bm = parsec_bm
        short = parsec_short
        suite = ["parsec"]*len(bm)
        suite_full = ["PARSEC 3.0 (Multi-threaded)"]*len(bm)
        # class_full = ["Multi-threaded"]*len(bm)
    elif suitename == "spec":
        bm = spec_bm
        short = spec_short
        suite = ["spec"]*len(bm)
        suite_full = ["SPEC CPU 2017 (Multi-programmed)"]*len(bm)
        # class_full = ["Multi-programmed"]*len(bm)
    elif suitename == "perfect":
        bm = perfect_bm
        short = perfect_short
        suite = ["perfect"]*len(bm)
        suite_full = ["PERFECT (Multi-threaded)"]*len(bm)
        # class_full = ["Multi-threaded"]*len(bm)
    elif suitename == "pp":
        bm = perfect_bm + parsec_bm
        short = perfect_short + parsec_short
        suite = ["perfect"]*len(perfect_bm) + ["parsec"]*len(parsec_bm)
        suite_full = ["PERFECT (Multi-threaded)"]*len(perfect_bm) + ["PARSEC 3.0 (Multi-threaded)"]*len(parsec_bm)
        # class_full = ["Multi-threaded"]*len(bm)
    elif suitename == "allsuite":
        bm = perfect_bm + parsec_bm + spec_bm
        short = perfect_short + parsec_short + spec_short
        suite = ["perfect"]*len(perfect_bm) + ["parsec"]*len(parsec_bm) + ["spec"]*len(spec_bm)
        suite_full = ["PERFECT (Multi-threaded)"]*len(perfect_bm) + ["PARSEC 3.0 (Multi-threaded)"]*len(parsec_bm) + ["SPEC CPU 2017 (Multi-programmed)"]*len(spec_bm)
        # class_full = ["Multi-threaded"]*len(perfect_bm) + ["Multi-threaded"]*len(parsec_bm) + ["Multi-programmed"]*len(spec_bm)
    else:
        print("Error: unknown suitename")
        sys.exit(1)
    
    if args.fig == 12:
        schemes_to_plot=[
            "bdi",
            # "XORCache(LSH-RP)",
            # "XORCache(LSH-BS)",
            # "XORCache(BL)", 
            "XORCache(SBL)",
            "idealBank", #oracle
        ]
    elif args.fig == 2: 
        schemes_to_plot=[
            "BDI (no XOR)",
            # "randSet",
            "XORCache+BDI randBank",
            "XORCache+BDI idealSet",
            "XORCache+BDI idealBank", #oracle
        ]
    elif args.fig == 4: 
        schemes_to_plot=[
            "BDI (no XOR)",
            # "randSet",
            "XORCache+BDI randBank",
            "XORCache+BDI idealSet-0",
            "XORCache+BDI idealSet-1",
            "XORCache+BDI idealSet-2",
            "XORCache+BDI idealSet-3",
            "XORCache+BDI idealSet-4",
            "XORCache+BDI idealBank", #oracle
        ]
    hashSchemeMaps = HashSchemeMaps(schemes_to_plot)
    
    result_dicts = [{}, {}, {}, {}]

    for scheme_to_plot in schemes_to_plot:
        inter_geomean_arr = []
        intra_geomean_arr = []
        results_geomean_arrs = [[],[],[], []]
        for bench, suitename_ in zip(bm, suite):
            
            vertical_slice_index = args.bit-1

            if suitename_ == "perfect":
                dumps = parse_perfect_snapshots(bench)
            elif suitename_ == "parsec":
                dumps = parse_parsec_snapshots(bench)
            elif suitename_ == "spec":
                dumps = parse_spec_snapshots(bench)
            else:
                assert False, print("Error: unknown suitename")

            results_geomean, stats_arr = get_geomean_cr(dumps, vertical_slice_index, scheme_to_plot, hashSchemeMaps)
            print(results_geomean)
            for i in range(len(results_geomean)):
                # if i == 0: results_geomean_arrs.append([])
                results_geomean_arrs[i].append(results_geomean[i])
        
        for i in range(len(result_dicts)):
            geomean = float(gmean(results_geomean_arrs[i]))
            arimean = float(sum(results_geomean_arrs[i])/len(results_geomean_arrs[i]))
            results_geomean_arrs[i].append(geomean)
            # results_geomean_arrs[i].append(arimean)
            
            result_dicts[i][scheme_to_plot] = results_geomean_arrs[i]
    # print(result_dicts)
    short += ["gm"]


    result_norm_dicts = [{}, {}, {}, {}]
    for i in range(len(result_dicts)):
        res_dict = result_dicts[i]
        key_norm = list(res_dict.keys())[0]
        value_norm = res_dict[key_norm]
        for key, value in res_dict.items():
            arr_normed = [float(x/value_norm[i]) for i, x in enumerate(value)]
            result_norm_dicts[i][key] = arr_normed

    if args.norm == True:
        final_res_dicts = result_norm_dicts
    else:
        final_res_dicts = result_dicts

    i=0
    ymax = 0
    ymin = 1
    if args.fig == 2:
        ylim = 4.09
    else:
        ylim = None
    for res_dict in final_res_dicts:
        fig = sp.make_subplots(rows=1, cols=1, 
                shared_yaxes=False,
                horizontal_spacing=0,
                vertical_spacing=0,)
        print(stats_arr[i])
        for key, value in res_dict.items():
            if args.onlygeomean:
                # short = short[-2:-1]
                # value = value[-2:-1]
                short = [""]
                value = [value[-1]]
            print(key, value)
            fig.add_trace(
                go.Bar(x=[suite_full+[""], [ str(i) for i in short]] if not args.onlygeomean else [[""], [""]], 
                       y=value,
                        name=key,
                        showlegend=True,
                        marker_color=scheme_to_color[key],
                        ),
                row=1, col=1)
            ymin = min(ymin, min(value))
            ymax = max(ymax, max(value))
            if ylim is not None:
                for x,y in zip(short, value):
                    if y > ylim:
                        fig.add_annotation(
                            x=value.index(y), 
                            y=ylim,
                            text=round(y,1),
                            showarrow=False,
                            textangle=-10,
                            xshift=45,
                            # xshift=(i-2)*14,
                            yshift=-10,
                            font=dict(size=22)
                        )
        if args.fig == 2:
            fig.add_annotation(
                x=29.3,
                y=3.1,
                xref="x",
                yref="y",
                text="",
                showarrow=True,
                font=dict(
                    family="Courier New, monospace",
                    # size=16,
                    # color="#ffffff"
                    ),
                align="center",
                arrowhead=2,
                arrowsize=1,
                arrowwidth=3,
                # arrowcolor="#636363",
                ax=-42,
                ay=120,
            )

            fig.add_annotation(
                x=29.1,
                y=3.4,
                text="2.08x",
                showarrow=False,
                textangle=0,
                xshift=0,
                yshift=0,
                font=dict(size=24)
            )
        fig.update_yaxes(range=[1, 1.05*ymax], row=1, col=1)
        if ylim is not None: fig.update_yaxes(range=[0, ylim], row=1, col=1)

        fig = format_fig(fig, args.onlygeomean, args.fig)
        #create dir
        os.makedirs(f'img_bar/fig{args.fig}/{suitename}', exist_ok=True)
        if args.fig == 12:
            if args.norm == True:
                if args.onlygeomean:
                    fig.write_image(f"img_bar/fig{args.fig}/{suitename}/bar{args.bit}-{stats_arr[i]}-norm-geomean.pdf")
                else:
                    fig.write_image(f"img_bar/fig{args.fig}/{suitename}/bar{args.bit}-{stats_arr[i]}-norm.pdf")
            else:
                if args.onlygeomean:
                    fig.write_image(f"img_bar/fig{args.fig}/{suitename}/bar{args.bit}-{stats_arr[i]}-geomean.pdf")
                else:
                    fig.write_image(f"img_bar/fig{args.fig}/{suitename}/bar{args.bit}-{stats_arr[i]}.pdf")
        else:
            if args.norm == True:
                if args.onlygeomean:
                    fig.write_image(f"img_bar/fig{args.fig}/{suitename}/bar-{stats_arr[i]}-norm-geomean.pdf")
                else:
                    fig.write_image(f"img_bar/fig{args.fig}/{suitename}/bar-{stats_arr[i]}-norm.pdf")
            else:
                if args.onlygeomean:
                    fig.write_image(f"img_bar/fig{args.fig}/{suitename}/bar-{stats_arr[i]}-geomean.pdf")
                else:
                    fig.write_image(f"img_bar/fig{args.fig}/{suitename}/bar-{stats_arr[i]}.pdf")
        i+=1