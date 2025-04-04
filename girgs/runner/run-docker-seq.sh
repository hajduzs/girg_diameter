#!/usr/bin/env bash
set -e
set -x

DATADIR=$(pwd)/data-seq-journal
mkdir -p $DATADIR

DOCKER_CMD="docker run --rm -it -u $(id -u ${USER}):$(id -g ${USER}) --hostname $(hostname) -e OMP_NUM_THREADS=1"

$DOCKER_CMD -v $DATADIR:/data girgs_embedder
$DOCKER_CMD -v $DATADIR:/data --entrypoint /usr/bin/benchmark-nkopt.sh -t girgs_hypergen
$DOCKER_CMD -v $DATADIR:/data --entrypoint /usr/bin/benchmark-hypergen.sh -t girgs_hypergen
$DOCKER_CMD -v $DATADIR:/data -t girgs_hypergirgs
$DOCKER_CMD -v $DATADIR:/data -t girgs_networkit
$DOCKER_CMD -v $DATADIR:/data -t girgs_nkbin
