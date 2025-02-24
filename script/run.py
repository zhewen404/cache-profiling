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

def launch(dir, launch_flag, num_banks=None, kb_per_bank=None):
    if not os.path.exists(dir):
        print("Error: directory does not exist")
        sys.exit(1)
    # launch the main program in background
    if num_banks and kb_per_bank:
        cmd = f"nohup ./bin/main {dir} {launch_flag} {num_banks} {kb_per_bank} >/dev/null 2>&1 &"
    elif num_banks and not kb_per_bank:
        cmd = f"nohup ./bin/main {dir} {launch_flag} {num_banks} >/dev/null 2>&1 &"
    elif not num_banks and not kb_per_bank:
        cmd = f"nohup ./bin/main {dir} {launch_flag} >/dev/null 2>&1 &"
    else: 
        print("Error: unknown arguments")
        sys.exit(1)
    print(f"Launching {cmd}")
    os.system(cmd)

def plot_hashfunction(dumps, benchname, suitename, schemes_to_plot=None, 
                      plot_even_only=False, plot_errorbar=True, plot_final=False, plot_num=0):
    hashSchemeMaps = HashSchemeMaps(schemes_to_plot)

    num_points = 0

    for d in dumps:
        for scheme in schemes_to_plot:
            # Construct file name dynamically based on scheme and dump directory
            scheme_name = hashSchemeMaps.scheme_to_name[scheme]
            file_path = f"{d}crss-{scheme_name}.txt"
            with open(file_path, "r") as file:
                lines = file.readlines()
                assert len(lines) == 3, print(f"len(lines) = {len(lines)}")
                crs_arr = [hashSchemeMaps.crs_schemes, hashSchemeMaps.crs_max_schemes, hashSchemeMaps.crs_min_schemes]
                for i in range(3):
                    l = lines[i]
                    scheme_vector = list(map(float, l.split()))
                    length = len(scheme_vector)
                    if len(scheme_vector) == 1:
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
                            scheme_vector = [np.nan] * 32
                    num_points = len(scheme_vector)
                    crs_arr[i][scheme].append(scheme_vector)

            # Repeat for ERS
            file_path = f"{d}erss-{scheme_name}.txt"
            with open(file_path, "r") as file:
                lines = file.readlines()
                assert len(lines) == 3
                ers_arr = [hashSchemeMaps.ers_schemes, hashSchemeMaps.ers_max_schemes, hashSchemeMaps.ers_min_schemes]
                for i in range(3):
                    l = lines[i]
                    scheme_vector = list(map(float, l.split()))
                    length = len(scheme_vector)
                    if len(scheme_vector) == 1:
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
                            scheme_vector = [np.nan] * 32
                    num_points = len(scheme_vector)
                    ers_arr[i][scheme].append(scheme_vector)

            # Repeat for FRS
            file_path = f"{d}frss-{scheme_name}.txt"
            with open(file_path, "r") as file:
                lines = file.readlines()
                assert len(lines) == 3
                frs_arr = [hashSchemeMaps.frs_schemes, hashSchemeMaps.frs_max_schemes, hashSchemeMaps.frs_min_schemes]
                for i in range(3):
                    l = lines[i]
                    scheme_vector = list(map(float, l.split()))
                    length = len(scheme_vector)
                    if len(scheme_vector) == 1:
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
                            scheme_vector = [np.nan] * 32
                    num_points = len(scheme_vector)
                    frs_arr[i][scheme].append(scheme_vector)

            # Repeat for INTRAS
            file_path = f"{d}intras-{scheme_name}.txt"
            with open(file_path, "r") as file:
                lines = file.readlines()
                assert len(lines) == 3
                intras_arr = [hashSchemeMaps.intras_schemes, hashSchemeMaps.intras_max_schemes, hashSchemeMaps.intras_min_schemes]
                for i in range(3):
                    l = lines[i]
                    scheme_vector = list(map(float, l.split()))
                    length = len(scheme_vector)
                    if len(scheme_vector) == 1:
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
                        else:
                            num = scheme_vector[0]
                            scheme_vector = [num] * 32
                    num_points = len(scheme_vector)
                    intras_arr[i][scheme].append(scheme_vector)

            # Repeat for HAMMINGS
            file_path = f"{d}hammings-{scheme_name}.txt"
            with open(file_path, "r") as file:
                lines = file.readlines()
                assert len(lines) == 3, f"len(lines) = {len(lines)}"
                hammings_arr = [hashSchemeMaps.hammings_schemes, hashSchemeMaps.hammings_max_schemes, hashSchemeMaps.hammings_min_schemes]
                for i in range(3):
                    l = lines[i]
                    scheme_vector = list(map(float, l.split()))
                    length = len(scheme_vector)
                    if len(scheme_vector) == 1:
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
                            scheme_vector = [np.nan] * 32
                    num_points = len(scheme_vector)
                    hammings_arr[i][scheme].append(scheme_vector)

    # Remaining code for processing and plotting the averages...
    print(len(hashSchemeMaps.crs_schemes[schemes_to_plot[0]]))
    xaxis = [i+1 for i in range(num_points)]

    crs_scheme_vs_avg = {}
    crs_max_scheme_vs_avg = {}
    crs_min_scheme_vs_avg = {}
    ers_scheme_vs_avg = {}
    ers_max_scheme_vs_avg = {}
    ers_min_scheme_vs_avg = {}
    frs_scheme_vs_avg = {}
    frs_max_scheme_vs_avg = {}
    frs_min_scheme_vs_avg = {}
    intras_scheme_vs_avg = {}
    intras_max_scheme_vs_avg = {}
    intras_min_scheme_vs_avg = {}
    hammings_scheme_vs_avg = {}
    hammings_max_scheme_vs_avg = {}
    hammings_min_scheme_vs_avg = {}

    crs_avg_arr = [crs_scheme_vs_avg, crs_max_scheme_vs_avg, crs_min_scheme_vs_avg]
    ers_avg_arr = [ers_scheme_vs_avg, ers_max_scheme_vs_avg, ers_min_scheme_vs_avg]
    frs_avg_arr = [frs_scheme_vs_avg, frs_max_scheme_vs_avg, frs_min_scheme_vs_avg]
    intras_avg_arr = [intras_scheme_vs_avg, intras_max_scheme_vs_avg, intras_min_scheme_vs_avg]
    hammings_avg_arr = [hammings_scheme_vs_avg, hammings_max_scheme_vs_avg, hammings_min_scheme_vs_avg]

    for avg, all in zip(crs_avg_arr, crs_arr):
        for scheme, data in all.items():
            if len(data) == 0: continue
            avg[scheme] = [gmean(x) for x in zip(*data)]
            if scheme == "ternarybytemap-shuffle-xorfold":
                new_vec = []
                for i in range(len(avg[scheme])):
                    if (i%2) != 0 and 2*i+1 < len(avg[scheme]): new_vec.append(avg[scheme][2*i+1])
                    else: new_vec.append(np.nan)
                avg[scheme] = new_vec
        
    for avg, all in zip(ers_avg_arr, ers_arr):
        for scheme, data in all.items():
            if len(data) == 0: continue
            avg[scheme] = [sum(x)/len(x) for x in zip(*data)]
            if scheme == "ternarybytemap-shuffle-xorfold":
                new_vec = []
                for i in range(len(avg[scheme])):
                    if (i%2) != 0 and 2*i+1 < len(avg[scheme]): new_vec.append(avg[scheme][2*i+1])
                    else: new_vec.append(np.nan)
                avg[scheme] = new_vec
        
    for avg, all in zip(frs_avg_arr, frs_arr):
        for scheme, data in all.items():
            if len(data) == 0: continue
            avg[scheme] = [sum(x)/len(x) for x in zip(*data)]
            if scheme == "ternarybytemap-shuffle-xorfold":
                new_vec = []
                for i in range(len(avg[scheme])):
                    if (i%2) != 0 and 2*i+1 < len(avg[scheme]): new_vec.append(avg[scheme][2*i+1])
                    else: new_vec.append(np.nan)
                avg[scheme] = new_vec
        
    for avg, all in zip(intras_avg_arr, intras_arr):
        for scheme, data in all.items():
            if len(data) == 0: continue
            avg[scheme] = [gmean(x) for x in zip(*data)]
            if scheme == "ternarybytemap-shuffle-xorfold":
                new_vec = []
                for i in range(len(avg[scheme])):
                    if (i%2) != 0 and 2*i+1 < len(avg[scheme]): new_vec.append(avg[scheme][2*i+1])
                    else: new_vec.append(np.nan)
                avg[scheme] = new_vec
        
    for avg, all in zip(hammings_avg_arr, hammings_arr):
        for scheme, data in all.items():
            if len(data) == 0: continue
            avg[scheme] = [sum(x)/len(x) for x in zip(*data)]
            if scheme == "ternarybytemap-shuffle-xorfold":
                new_vec = []
                for i in range(len(avg[scheme])):
                    if (i%2) != 0 and 2*i+1 < len(avg[scheme]): new_vec.append(avg[scheme][2*i+1])
                    else: new_vec.append(np.nan)
                avg[scheme] = new_vec

    if plot_even_only:
        xaxis = xaxis[1::2]
        for scheme in crs_scheme_vs_avg:
            crs_scheme_vs_avg[scheme] = crs_scheme_vs_avg[scheme][1::2]
            crs_max_scheme_vs_avg[scheme] = crs_max_scheme_vs_avg[scheme][1::2]
            crs_min_scheme_vs_avg[scheme] = crs_min_scheme_vs_avg[scheme][1::2]

        for scheme in ers_scheme_vs_avg:
            ers_scheme_vs_avg[scheme] = ers_scheme_vs_avg[scheme][1::2]
            ers_max_scheme_vs_avg[scheme] = ers_max_scheme_vs_avg[scheme][1::2]
            ers_min_scheme_vs_avg[scheme] = ers_min_scheme_vs_avg[scheme][1::2]

        for scheme in frs_scheme_vs_avg:
            frs_scheme_vs_avg[scheme] = frs_scheme_vs_avg[scheme][1::2]
            frs_max_scheme_vs_avg[scheme] = frs_max_scheme_vs_avg[scheme][1::2]
            frs_min_scheme_vs_avg[scheme] = frs_min_scheme_vs_avg[scheme][1::2]

        for scheme in intras_scheme_vs_avg:
            intras_scheme_vs_avg[scheme] = intras_scheme_vs_avg[scheme][1::2]
            intras_max_scheme_vs_avg[scheme] = intras_max_scheme_vs_avg[scheme][1::2]
            intras_min_scheme_vs_avg[scheme] = intras_min_scheme_vs_avg[scheme][1::2]

        for scheme in hammings_scheme_vs_avg:
            hammings_scheme_vs_avg[scheme] = hammings_scheme_vs_avg[scheme][1::2]
            hammings_max_scheme_vs_avg[scheme] = hammings_max_scheme_vs_avg[scheme][1::2]
            hammings_min_scheme_vs_avg[scheme] = hammings_min_scheme_vs_avg[scheme][1::2]

    import plotly.graph_objects as go
    import plotly.subplots as sp
    import pandas as pd
    import plotly.io as pio
    pio.kaleido.scope.mathjax = None

    if plot_final: 
        if plot_num == 0:
            col_ct = 3
        elif plot_num == 1:
            col_ct = 2
        elif plot_num == 2:
            col_ct = 1
    else: col_ct = 7
    
    if plot_final: spacing = 0.1
    else: spacing = 0.06

    fig = sp.make_subplots(rows=1, cols=col_ct, 
                        #    subplot_titles=("Compression Ratio", "Entropy Reduction", "False Positive Rate"),
                           shared_yaxes=False,
                           horizontal_spacing=spacing,
                           vertical_spacing=0,)
    
    if plot_final:
        color_sequence = [scheme_to_color[s] for s in schemes_to_plot]
    else:
        color_sequence = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', 
                        '#8c564b', '#e377c2', '#7f7f7f', '#bcbd22', '#17becf',]
        color_sequence = [scheme_to_color[s] for s in schemes_to_plot]

    from calc_xor_cache_storage_breakdown import calc_map_table

    if plot_num != 2:
        i = 0
        for i in range(len(crs_scheme_vs_avg.items())):
                scheme = list(crs_scheme_vs_avg.keys())[i]
                avg_ = list(crs_scheme_vs_avg.values())
                max_ = list(crs_max_scheme_vs_avg.values())
                min_ = list(crs_min_scheme_vs_avg.values())

                if "bdi" in scheme or "ideal" in scheme or "oracle" in scheme or "BDI (no XOR)" in scheme:
                    mode = "lines"
                else: 
                    mode = "markers+lines"
                
                if "ideal" in scheme or "oracle" in scheme:
                    dash = "dash"
                else:
                    dash = "solid"

                fig.add_trace(
                    go.Scatter(x=xaxis, y=avg_[i],
                            mode=mode,
                            name=scheme,
                            legendgroup=scheme,
                            line=dict(color=color_sequence[i], dash=dash), 
                            ),
                    row=1, col=1)
                if plot_errorbar:
                    fig.add_trace(
                        go.Scatter(x=xaxis, y=max_[i],
                                mode='lines',
                                showlegend=False,
                                legendgroup=scheme,
                                line=dict(color=color_sequence[i], width=0), 
                                ),
                        row=1, col=1)
                    fig.add_trace(
                        go.Scatter(x=xaxis, y=min_[i],
                                mode='lines',
                                showlegend=False,
                                fill='tonexty',
                                legendgroup=scheme,
                                line=dict(color=color_sequence[i], width=0), 
                                ),
                        row=1, col=1)
                i += 1
        fig.update_xaxes(title="(a)", row=1, col=1)
        fig.update_yaxes(title="Inter-line comp. ratio", row=1, col=1)

        i=0
        for i in range(len(intras_scheme_vs_avg.items())):
                scheme = list(intras_scheme_vs_avg.keys())[i]
                avg_ = list(intras_scheme_vs_avg.values())
                max_ = list(intras_max_scheme_vs_avg.values())
                min_ = list(intras_min_scheme_vs_avg.values())

                if "bdi" in scheme or "ideal" in scheme or "oracle" in scheme or "BDI (no XOR)" in scheme: 
                    mode = "lines"
                else: mode = "markers+lines"
                
                if "ideal" in scheme or "oracle" in scheme:
                    dash = "dash"
                else:
                    dash = "solid"

                fig.add_trace(
                    go.Scatter(x=xaxis, y=avg_[i],
                            mode=mode,
                            name=scheme,
                            showlegend=False,
                            legendgroup=scheme,
                            line=dict(color=color_sequence[i], dash=dash), 
                            ),
                    row=1, col=2)
                
                if plot_errorbar:
                    fig.add_trace(
                        go.Scatter(x=xaxis, y=max_[i],
                                mode='lines',
                                showlegend=False,
                                legendgroup=scheme,
                                line=dict(color=color_sequence[i], width=0), 
                                ),
                        row=1, col=2)
                    fig.add_trace(
                        go.Scatter(x=xaxis, y=min_[i],
                                mode='lines',
                                showlegend=False,
                                fill='tonexty',
                                legendgroup=scheme,
                                line=dict(color=color_sequence[i], width=0), 
                                ),
                        row=1, col=2)
                i += 1
        fig.update_yaxes(title="Intra-line comp. ratio", row=1, col=2, title_standoff = 0)
        fig.update_xaxes(title="(b)", row=1, col=2)

    if plot_num == 1:
        pass
    else:
        if plot_num == 2: 
            colid = 1
        else: colid = 3
        i=0
        for i in range(len(intras_scheme_vs_avg.items())):
            scheme = list(intras_scheme_vs_avg.keys())[i]
            avg_intra = list(intras_scheme_vs_avg.values())
            avg_inter = list(crs_scheme_vs_avg.values())
            avg_ = []
            for ii in range(len(avg_intra)):
                avg_.append([])
                for jj in range(len(avg_intra[ii])):
                    avg_[ii].append(avg_intra[ii][jj] * avg_inter[ii][jj])

            if "bdi" in scheme or "ideal" in scheme or "oracle" in scheme or "BDI (no XOR)" in scheme: 
                mode = "lines"
            else: mode = "markers+lines"
            
            if "ideal" in scheme or "oracle" in scheme:
                dash = "dash"
            else:
                dash = "solid"

            fig.add_trace(
                go.Scatter(x=xaxis, y=avg_[i],
                        mode=mode,
                        name=scheme,
                        showlegend=False,
                        legendgroup=scheme,
                        line=dict(color=color_sequence[i], dash=dash), 
                        ),
                row=1, col=colid)
            i += 1
        if plot_num == 2: 
            fig.update_yaxes(title="Total comp. ratio", row=1, col=colid)
            h = 1
        else:
            fig.update_yaxes(title="Total comp. ratio", row=1, col=colid, title_standoff = 0)
        fig.update_xaxes(title="(c)", row=1, col=colid)
    
    if not plot_final:

        size_in_kb = [[]]
        assoc_arr = [1]
        i=0
        for assoc in assoc_arr:
            size_in_kb_ = []
            for hash_fingerprint_size_in_bits in xaxis:
                # if hash_fingerprint_size_in_bits < 12: 
                #     map_table_num_entries = pow(2, hash_fingerprint_size_in_bits)
                # else:
                #     map_table_num_entries = 4096
                map_table_num_entries = pow(2, hash_fingerprint_size_in_bits)
                
                map_table_assoc = assoc
                map_table_num_sets = map_table_num_entries / map_table_assoc

                map_table_num_entries, map_table_entry_size_in_bits, map_table_storage_in_KiB = \
                            calc_map_table(1024, hash_fingerprint_size_in_bits, map_table_num_sets, map_table_assoc)
                size_in_kb_.append(map_table_storage_in_KiB)
            size_in_kb.append(size_in_kb_)
            fig.add_trace(
                go.Scatter(x=xaxis, y=size_in_kb_,
                        mode="markers+lines",
                        name="size",
                        showlegend=False,
                        line=dict(color="black"), 
                        ),
                row=1, col=7)
            i+=1
        fig.update_yaxes(title="Map table size (KiB)", row=1, col=7, title_standoff = 0,type="log")

        i=0
        for i in range(len(ers_scheme_vs_avg.items())):
                scheme = list(ers_scheme_vs_avg.keys())[i]
                avg_ = list(ers_scheme_vs_avg.values())
                max_ = list(ers_max_scheme_vs_avg.values())
                min_ = list(ers_min_scheme_vs_avg.values())

                if "bdi" in scheme or "ideal" in scheme or "oracle" in scheme or "BDI (no XOR)" in scheme: 
                    mode = "lines"
                else: mode = "markers+lines"
            
                if "ideal" in scheme or "oracle" in scheme:
                    dash = "dash"
                else:
                    dash = "solid"

                fig.add_trace(
                    go.Scatter(x=xaxis, y=avg_[i],
                            mode=mode,
                            name=scheme,
                            showlegend=False,
                            legendgroup=scheme,
                            line=dict(color=color_sequence[i], dash=dash), 
                            ),
                    row=1, col=5)
                
                if plot_errorbar:
                    fig.add_trace(
                        go.Scatter(x=xaxis, y=max_[i],
                                mode='lines',
                                showlegend=False,
                                legendgroup=scheme,
                                line=dict(color=color_sequence[i], width=0), 
                                ),
                        row=1, col=5)
                    fig.add_trace(
                        go.Scatter(x=xaxis, y=min_[i],
                                mode='lines',
                                showlegend=False,
                                fill='tonexty',
                                legendgroup=scheme,
                                line=dict(color=color_sequence[i], width=0), 
                                ),
                        row=1, col=5)
                i += 1
        fig.update_yaxes(title="entropy reduction", row=1, col=5, title_standoff = 0)
    
        i=0
        for i in range(len(frs_scheme_vs_avg.items())):
                scheme = list(frs_scheme_vs_avg.keys())[i]
                avg_ = list(frs_scheme_vs_avg.values())
                max_ = list(frs_max_scheme_vs_avg.values())
                min_ = list(frs_min_scheme_vs_avg.values())

                if "bdi" in scheme or "ideal" in scheme or "oracle" in scheme or "BDI (no XOR)" in scheme: 
                    mode = "lines"
                else: mode = "markers+lines"
            
                if "ideal" in scheme or "oracle" in scheme:
                    dash = "dash"
                else:
                    dash = "solid"

                fig.add_trace(
                    go.Scatter(x=xaxis, y=avg_[i],
                            mode=mode,
                            name=scheme,
                            showlegend=False,
                            legendgroup=scheme,
                            line=dict(color=color_sequence[i], dash=dash), 
                            ),
                    row=1, col=4)
                
                if plot_errorbar:
                    fig.add_trace(
                        go.Scatter(x=xaxis, y=max_[i],
                                mode='lines',
                                showlegend=False,
                                legendgroup=scheme,
                                line=dict(color=color_sequence[i], width=0), 
                                ),
                        row=1, col=4)
                    fig.add_trace(
                        go.Scatter(x=xaxis, y=min_[i],
                                mode='lines',
                                showlegend=False,
                                fill='tonexty',
                                legendgroup=scheme,
                                line=dict(color=color_sequence[i], width=0), 
                                ),
                        row=1, col=4)
                i += 1
        fig.update_yaxes(title="false positive rate", row=1, col=4, title_standoff = 0)

        i=0
        for i in range(len(hammings_scheme_vs_avg.items())):
                scheme = list(hammings_scheme_vs_avg.keys())[i]
                avg_ = list(hammings_scheme_vs_avg.values())
                max_ = list(hammings_max_scheme_vs_avg.values())
                min_ = list(hammings_min_scheme_vs_avg.values())

                if "bdi" in scheme or "ideal" in scheme or "oracle" in scheme or "BDI (no XOR)" in scheme: 
                    mode = "lines"
                else: mode = "markers+lines"
            
                if "ideal" in scheme or "oracle" in scheme:
                    dash = "dash"
                else:
                    dash = "solid"

                fig.add_trace(
                    go.Scatter(x=xaxis, y=avg_[i],
                            mode=mode,
                            name=scheme,
                            showlegend=False,
                            legendgroup=scheme,
                            line=dict(color=color_sequence[i], dash=dash), 
                            ),
                    row=1, col=6)
                
                if plot_errorbar:
                    fig.add_trace(
                        go.Scatter(x=xaxis, y=max_[i],
                                mode='lines',
                                showlegend=False,
                                legendgroup=scheme,
                                line=dict(color=color_sequence[i], width=0), 
                                ),
                        row=1, col=6)
                    fig.add_trace(
                        go.Scatter(x=xaxis, y=min_[i],
                                mode='lines',
                                showlegend=False,
                                fill='tonexty',
                                legendgroup=scheme,
                                line=dict(color=color_sequence[i], width=0), 
                                ),
                        row=1, col=6)
                i += 1
        fig.update_yaxes(title="hamming distance", row=1, col=6, title_standoff = 0)

        

    dpi = 300
    w=3.3115
    if plot_num !=2:h=1.4
    else: h=1
    fig.update_yaxes(ticks="outside",
                 ticklen=0,  # adjust length of the tick = distance from axis
                 )
    # make the legend horizontal at the bottom
    if plot_final: ew = 240
    else: ew=None
    fig.update_layout(
        legend=dict(
        entrywidth=ew,
        # entrywidthmode='fraction',
        orientation="h",
        yanchor="bottom",
        y=-0.55,
        xanchor="right",
        x=1,
        font=dict(
            size=22,
        )
    ))

    if plot_final: width = w
    else: width = 2*w

    fig.update_layout(
        template="plotly_white",
        width=width*dpi,
        height=h*dpi,
        paper_bgcolor='rgba(0,0,0,0)',
        title=None,
        # xaxis_title=None,
        margin=dict(l=2, r=2, t=2, b=2),
        font=dict(
            family="ubuntu",
            size=22,
        ),
        bargap=0.10,
        bargroupgap=0.0,
    )

    os.makedirs(f'img/{suitename}', exist_ok=True)
    if plot_even_only:
        fig.write_image(f"img/{suitename}/hash-{benchname}-even.pdf")
        print(f"img/{suitename}/hash-{benchname}-even.pdf written")
    else:
        if plot_final: 
            fig.write_image(f"img/{suitename}/hash-{benchname}-final{plot_num}.pdf")
            print(f"img/{suitename}/hash-{benchname}-final{plot_num}.pdf written")
        else: 
            fig.write_image(f"img/{suitename}/hash-{benchname}.pdf")
            print(f"img/{suitename}/hash-{benchname}.pdf written")

def plot_profiling(dumps, benchname, suitename, stats_to_plot=None):
    profileStatMaps = ProfileStatMaps(stats_to_plot)
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
            
            profileStatMaps.stat_arr[stat].append(stat_vector)
    # #print stat_arr
    # for stat, data in stat_arr.items():
    #     print(stat, len(data))
    #     print(data[0])

    # Remaining code for processing and plotting the averages...
    print(len(profileStatMaps.stat_arr[stats_to_plot[0]]))

    stats_avg = {}

    for stat, data in profileStatMaps.stat_arr.items():
        if len(data) == 0: continue
        # stats_avg[stat] = [gmean(x) for x in zip(*data)]
        if "histogram" in stat:
            stats_avg[stat] = [sum(x) for x in zip(*data)]
        else:
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
    fig = sp.make_subplots(rows=1, 
                            # cols=len(stats_to_plot),
                            cols= 1, 
                        #    subplot_titles=("Compression Ratio", "Entropy Reduction", "False Positive Rate"),
                           shared_yaxes=False,
                           horizontal_spacing=0.15,
                           vertical_spacing=0,)

    color_sequence = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', 
                      '#8c564b', '#e377c2', '#7f7f7f', '#bcbd22', '#17becf']
    
    num = 1
    for stat, data in stats_avg.items():
        if "histogram" in stat:
            fig.add_trace(
                go.Bar(x=list(range(len(data))),
                        y=data,
                        marker=dict(color=color_sequence[0]),
                        showlegend=False,
                        ),

                row=1, col=num)
        else:
            fig.add_trace(
                go.Scatter(x=list(range(len(data))),
                        y=data,
                        mode='markers+lines',
                        name=stat,
                        # legendgroup=num,
                        line=dict(color=color_sequence[num]), 
                        # showlegend=False,
                        ),

                # row=1, col=num)
                row=1, col=1)
        # fig.update_yaxes(title=stat, row=1, col=num)

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
        elif "hamming byte position" in stat:
            # plot word boundary as vertical lines
            for i in range(0, 64, 8):
                fig.add_shape(
                    # Line Vertical
                    dict(
                        type="line",
                        x0=i,
                        y0=0,
                        x1=i,
                        y1=255,
                        line=dict(
                            color="Black",
                            width=1,
                            dash="dot",
                        )
                    ),
                    # row=1, col=num
                    row=1, col=1
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
                    zmax=255,
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
            fig_new.write_image(f"img/{suitename}/profiling-{stat_to_name[stat]}-{benchname}-word-byte-hamming-heatmap.pdf")

            fig_cs = go.Figure()
            # plot a vector heatmap with min=0 and max=8
            # annotate the heatmap with the data
            fig_cs.add_trace(
                go.Heatmap(
                    z=[np.linspace(0, 255, 80)],
                    # text=[[7,6,5,4,3,2,1,0]],
                    # texttemplate="%{text}",
                    # textfont={"size":20},
                    colorscale='RdBu_r',
                    zmin=0,
                    zmax=255,
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
                ticktext=["0", "127", "255"],
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
            fig_cs.write_image(f"img/{suitename}/profiling-{stat_to_name[stat]}-{benchname}-word-byte-hamming-heatmap-cs.pdf")
        elif "heatcube" in  stat:
            import matplotlib.pyplot as plt
            # print(data)
            # print(len(data))
            data = data[0:9*9*9]
            # covert data to log scale
            data = [x+0.000000000001 if x == 0 else x for x in data ]
            data = [np.log(x) for x in data]

            data = np.array(data).reshape(9, 9, 9)
            x = y = z = np.linspace(0, 8, 9)
            X, Y, Z = np.meshgrid(x, y, z)
            values = data

            fig_plt = plt.figure()
            ax = fig_plt.add_subplot(111, projection='3d')

            ax.set_xlabel('x')
            ax.set_ylabel('y')
            ax.set_zlabel('z')

            scatter = ax.scatter(X, Y, Z, c=values, cmap='Reds')
            fig_plt.colorbar(scatter, ax=ax)

            xx, yy = np.meshgrid(range(9), range(9))
            # zz is max of xx and yy
            zz_max = np.maximum(xx, yy)
            zz_min = np.minimum(xx, yy)
            # plot the plane
            ax.plot_surface(xx, yy, zz_max, alpha=0.5, color="blue")
            ax.plot_surface(xx, yy, zz_min, alpha=0.5, color="green")

            plt.show()
            plt.savefig(f"img/{suitename}/profiling-{stat_to_name[stat]}-{benchname}-heatcube.pdf")
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
    os.makedirs(f'img/{suitename}', exist_ok=True)
    fig.write_image(f"img/{suitename}/profiling-{benchname}.pdf")

if __name__ == "__main__":
    # add argument to specify suitename, launch_flag, and plot
    parser = argparse.ArgumentParser(description='run and plot script')
    parser.add_argument('--suitename', 
                        help='suitename',
                        choices=["parsec", "spec", "perfect", "allsuite", "pp"])
    parser.add_argument('--launch', 
                        help='launch the simulations for scheme',
                        default=None)
    parser.add_argument('--plot', 
                        help='plot the results',
                        default="hash",
                        choices=["hash", "profiling"])
    parser.add_argument('--benchname',
                        help='benchmark name',
                        default="*")
    parser.add_argument('--plot_final',
                        help='plot the final results',
                        action='store_true')
    parser.add_argument('--plot_num',
                        type = int,
                        default=0)
    args = parser.parse_args()

    benchname = args.benchname
    suitename = args.suitename
    launch_flag = args.launch
    # plot="profiling"
    plot=args.plot

    num_banks = None
    kb_per_bank = None
    if suitename == "parsec":
        dumps = parse_parsec_snapshots(benchname)
        num_banks = 4
        kb_per_bank = 64
    elif suitename == "spec":
        dumps = parse_spec_snapshots(benchname)
        num_banks = 4
        kb_per_bank = 64
    elif suitename == "perfect":
        dumps = parse_perfect_snapshots(benchname)
        # num_banks = 1
        num_banks = 4
        kb_per_bank = 64
    elif suitename == "pp":
        dumps = parse_parsec_snapshots("*") + parse_perfect_snapshots("*")
    elif suitename == "allsuite":
        dumps = parse_parsec_snapshots("*") + parse_spec_snapshots("*") + parse_perfect_snapshots("*")
    else:
        print("Error: unknown suitename")
        sys.exit(1)

    if launch_flag:
        assert suitename != "allsuite"
        for d in dumps:
            launch(d, launch_flag, num_banks, kb_per_bank)
        print(len(dumps))

    if launch_flag == None:
        if benchname == "*" or suitename == "allsuite": benchname = "all"
        if plot == "profiling":
            plot_profiling(dumps, benchname, suitename,
                           stats_to_plot=[
                               "entropy byte position",
                            #    "entropy byte position<br>after oracle",
                            #    "entropy byte position<br>after xor rand bank",
                            #    "entropy byte position<br>after xor bytemap",
                            #    "entropy byte position<br>after xor bytemap only xored",
                            #    "entropy byte position<br>after xor sparseshuffledbytemap_4_3",
                            #    "entropy byte position<br>after xor sparseshuffledbytemap_4_3 only xored",
                            #    "entropy byte position<br>after xor thesaurus",
                            #    "entropy byte position<br>after xor lowentropy_8_16(BCD)",
                            #    "entropy byte position<br>after xor lowentropy_8_16(BCD) only xored",
                            #    "histogram word pattern epc",
                            #    "histogram word pattern strong",
                            #    "histogram word pattern hycomp",
                            #    "histogram word pattern semantic",
                            #    "histogram word pattern density",
                            #    "histogram word pattern averagebytemsb 4",
                            #    "histogram word pattern averagebytemsb 3",
                            #    "histogram word pattern averagebytemsb 2",

                            #    "hamming byte position<br>after oracle",
                            #    "hamming byte position<br>after xor bytemap",

                            #    "heatcube size change<br>after oracle",
                           ])
        elif plot == "hash":
            plot_hashfunction(dumps, benchname, suitename, 
                    schemes_to_plot=[
                        "XOR idealBank+BDI", #oracle
                        # "bpc",
                        # "Bitplane (no XOR)",
                        # "bdi",
                        "BDI (no XOR) ",
                        # "BDI",
                        # "bdi-immo",
                        # "thesaurus-flat",

                        # "XORCache(LSH-RP)",
                        "XOR(LSH-RP)+BDI",

                        # "shuffle-xorfold",
                        # "bit-sampling-flat",

                        # "XORCache(LSH-BS)",
                        "XOR(LSH-BS)+BDI",

                        # # "masked-bit-sampling_8_32",
                        # # "masked-bit-sampling_4_16",
                        # # "masked-bit-sampling_8_16",
                        # # "masked-bit-sampling_4_8",
                        # # "thesaurus-immo",
                        # # "maxbytemap-shuffle-xorfold", 
                        # # "bytemap-shuffle-xorfold-bpc", 
                        # # "bytemap-shuffle-xorfold-immo", 
                        # # "sparseshuffledbytemap_8_7",
                        # # "sparseshuffledbytemap_8_4",
                        # # "sparseshuffledbytemap_4_3",
                        # # "sparseshuffledbytemap_4_3-bpc",
                        # # "sparseshuffledbytemap_4_2",
                        # # "ternarybytemap-shuffle-xorfold",
                        # # "lowentropy_8_4",
                        # # "lowentropy_8_16(BCD)",
                        # # "lowentropy_8_16(BCD)-immo",
                        # # "EPC word labeling-flat",
                        # # "EPC word labeling",
                        # # "strong word labeling",
                        # # "hycomp word labeling-flat",
                        # # "hycomp word labeling",
                        # # "semantic word labeling",
                        # # "density word labeling",
                        # # "density word labeling bpc",
                        # # "average byte msb(4) word labeling",
                        # # "average byte msb(3) word labeling",
                        # # "average byte msb(2) word labeling",
                        # # "byte labeling-flat", 

                        # "XORCache(BL)", 
                        "XOR(BL)+BDI", 

                        # # "sparsebytemap(8,6)-shuffle-xorfold-flat",
                        
                        # "XORCache(SBL)",
                        "XOR(SBL)+BDI",

                        # # "XORCache(SBL)-1",
                        # # "XORCache(SBL)-2",
                        # # "XORCache(SBL)-3",
                        # # "XORCache(SBL)-4",
                        
                        # # "bdi line labeling",
                        # # "bdi-immo line labeling",
                        # # "bpc line labeling",

                        # "idealBank", #oracle
                        # "BPC (no XOR)",
                        # "XORCache+BPC(LSH-RP)",
                        # "XORCache+BPC(LSH-BS)",
                        # "XORCache+BPC(BL)", 
                        # "XORCache+BPC(SBL)",
                        # "masked-bit-sampling_8_16 (BPC)"
                        # "EPC word labeling-BPC",
                        # "strong word labeling-BPC",
                        # "hycomp word labeling (BPC)",
                        ],
                    plot_final=args.plot_final,
                    plot_errorbar=False,
                    plot_num=args.plot_num,
                    )
