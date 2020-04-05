CURR_DIR=$(dirname "$0")
cd $CURR_DIR
git fetch
git lfs fetch --recent
git pull
git submodule update --init --recursive
