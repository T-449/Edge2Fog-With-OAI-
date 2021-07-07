# colors
txtblk='\033[0;30m' # Black - Regular
txtred='\033[0;31m' # Red
txtgrn='\033[0;32m' # Green
txtblu='\033[0;34m' # Blue
txtpur='\033[0;35m' # Purple
txtwht='\033[0;37m' # White
txtrst='\033[0m'    # Text Reset


echo -e ${txtblu}Stopping SPGWU container${txtrst}
sudo docker stop prod-oai-spgwu-tiny
sudo docker container rm  prod-oai-spgwu-tiny


echo -e ${txtblu}Creating SPGWU container${txtrst}
python3 component/oai-spgwu-tiny/ci-scripts/generateConfigFiles.py --kind=SPGW-U \
          --sxc_ip_addr=192.168.61.4 --sxu=eth0 --s1u=eth0 \
          --from_docker_file --env_for_entrypoint

sudo docker run --privileged --name prod-oai-spgwu-tiny --network prod-oai-public-net --ip 192.168.61.5 --env-file ./spgwu-env.list oai-spgwu-tiny:production
