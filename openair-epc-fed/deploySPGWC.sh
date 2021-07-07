# colors
txtblk='\033[0;30m' # Black - Regular
txtred='\033[0;31m' # Red
txtgrn='\033[0;32m' # Green
txtblu='\033[0;34m' # Blue
txtpur='\033[0;35m' # Purple
txtwht='\033[0;37m' # White
txtrst='\033[0m'    # Text Reset


echo -e ${txtblu}Stopping SPGWC container${txtrst}
sudo docker stop prod-oai-spgwc
sudo docker container rm  prod-oai-spgwc



echo -e ${txtblu}Creating SPGWC container${txtrst}
python3 component/oai-spgwc/ci-scripts/generateConfigFiles.py --kind=SPGW-C \
          --s11c=eth0 --sxc=eth0 --apn=apn1.carrier.com \
          --dns1_ip=8.8.8.8 --dns2_ip=8.8.4.4 \
          --from_docker_file --env_for_entrypoint

sudo docker run --privileged --name prod-oai-spgwc --network prod-oai-public-net --ip 192.168.61.4 --env-file ./spgwc-env.list oai-spgwc:production
