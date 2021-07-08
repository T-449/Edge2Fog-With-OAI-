Install the following in the venv of OIDC_Client and Proxy2:
	django==2.2.12
	django-oidc-provider


Pre-deployment:

1) Provide IP of your device in openair-epc-fed/component/oai-mme/src/nas/emm/nas_emm_task.c: 164 (server.sin_addr.s_addr = inet_addr("IP of your device"))

2) Build the MME using ./buildMME.sh from within openair-epc-fed directory

3) Provide IP of your device in Proxy2/django-oidc-provider/example/receiverReply.py: 9 (vUser_IP = 'IP of your device')



Deployment:

1) Run ./deployHSS.sh and ./deployMME.sh from within openair-epc-fed directory

2) Run all receiverReply and receiverRequest files in OIDC_Client and Proxy2

3) Run tempUE from OIDC_Client

4) Run ./resCalculation from within ResCalculator directory

5) Run django server at port 9999 for OIDC_Client

6) Run django server at default port(8000) for Proxy2

7) Go to localhost:9999/client from browser


Observation:

The login button in the localhost:9999/client page will be disabled. Press 's' in the temporary UE. Once the UE is authenticated the login button will be enabled in the same page.
