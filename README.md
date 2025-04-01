# Diameter Calculation in GIRGs

This repository contains code, based on the original implementation by Thomas Bläsius and Philipp Fischbeck, to reproduce our experiments from our semester project at ETH. In this project, we investigated the iFUB algorithm and the diameter in the GIRG model. 

Authors: Zsombor Hajdu, Haitong Liu

Supervisor: Johannes Lengler

For a comprehensive overview of the code structure and functionality, please refer to the excellent [repository](https://github.com/thobl/external-validity) of the original authors. Here, we describe how to accommodate new experiments and how to utilize the [Euler cluster](https://scicomp.ethz.ch/wiki/Euler) to run the experiments.

## Adding New Experiments

To incorporate a new experiment, the following modifications are required:

- Add the necessary modifications to `code/src/` in appropriate locations (create new files if needed) and create corresponding headers in `code/include/`.
- Implement the CLI application code and place it under `code/cli/`.
- Add the experiment call to `experiments.py`, using existing entries as templates.
- Update `code/CMakeLists.txt` to include the newly added files.

## Using Euler

Once the codebase is prepared for new experiments and tested locally, upload it to your `$SCRATCH` folder on Euler (excluding `input_data`, `output_data`, and `code/release` directories, as they contain the experiment data / binaries).

On Euler, navigate to the directory containing the code and load the necessary modules:

```sh
module load stack/2024-06 gcc/12.2.0 python/3.11.6 cmake/3.27.7
```

Then, build the project using:

```sh
sh ./build.sh
```

And create the dictionary for the script to generate the raw graphs into:

```sh
mkdir input_data
mkdir input_data/raw
```

To run the project, use `sbatch`:

```sh
sbatch -n 6 --time=6:00:00 --mem-per-cpu=8000 --wrap="python3 ./experiments.py gen_girg prep diameter_exact merge_csv_force"
```

This will create a job that runs in the background. The job's progress and intermediate results are logged in the `slurm-<job_id>.out` file.

Once the results are available, download only the merged CSV files to your local machine for plotting using `scp` or any other preferred method, as described in the Euler documentation.

## Plotting

After running the experiments, plots can be generated using `plotting.ipynb`. Simply set the correct paths and execute the cells sequentially from the beginning. Note that some plots use data flitered for certain paramaters or other information - these can be usuallly set in separate cells, appearing right before a given plotting script.

## Other Remarks

- Our report includes findings on the spatial distribution of starting vertices. While we have uploaded the CSV containing the original data, we have not included the code that generated it. The reason is that our approach required extending the original graph class to store geometric vertex information, necessitating modifications not only to the core codebase but also to the GIRGs submodule. Since these findings were not critical to the analysis, we decided not to include these fairly complicated (and inefficent) modifications.
- Some of the original experiments were unnecessary for our purposes. To speed up compilation, we removed them from `CMakeLists.txt` and excluded certain format generations from `prepare_graph_formats.cpp`. If you wish to reintroduce these experiments, uncomment the relevant lines in the `prepare_graph_formats.cpp` file and restore the necessary entries in `CMakeLists.txt`. When in doubt, refer to the original repository.
- The uploaded data does not cover all possible parameter-parameter or parameter-experiment combinations. If a more comprehensive analysis is needed, you may need to generate additional graphs and rerun the experiments.

In case of any questions, write an email to `zhajdu` (under the ETH mail domain)
