# colors
txtblk='\033[0;30m' # Black - Regular
txtred='\033[0;31m' # Red
txtgrn='\033[0;32m' # Green
txtblu='\033[0;34m' # Blue
txtpur='\033[0;35m' # Purple
txtwht='\033[0;37m' # White
txtrst='\033[0m'    # Text Reset

echo -e ${txtblu}Stopping MME container${txtrst}
sudo docker stop prod-oai-mme
sudo docker container rm  prod-oai-mme


echo -e ${txtblu}Creating MME container${txtrst}
python3 component/oai-mme/ci-scripts/generateConfigFiles.py --kind=MME \
          --hss_s6a=192.168.61.2 --mme_s6a=192.168.61.3 \
          --mme_s1c_IP=192.168.61.3 --mme_s1c_name=eth0 \
          --mme_s10_IP=192.168.61.3 --mme_s10_name=eth0 \
          --mme_s11_IP=192.168.61.3 --mme_s11_name=eth0 \
          --spgwc0_s11_IP=192.168.61.4 \
          --mcc=320 --mnc=230 --tac_list="5 6 7" \
          --from_docker_file --env_for_entrypoint
sudo docker run --privileged --name prod-oai-mme --network prod-oai-public-net --ip 192.168.61.3 \
             --env-file ./mme-env.list oai-mme:production
