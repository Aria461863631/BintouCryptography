import csv
import base64
from asn1crypto.x509 import Certificate
import mysql.connector

# Open data set
with open('./2018-10-23-1540260352-https_get_443_certs') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')

    cnt = 1
    fo = open("Inpt_int.modulus", "w")
    # Parse every row
    for row in csv_reader:
        # Load a certificate
        try:
            cert = Certificate.load(base64.b64decode(row[1]))
        except:
            continue

        # If cert.subject has `native`
        try:
            dict = cert.subject.native
        except:
            continue

        # If the subject of the certificate has common name, parse it!
        if 'common_name' in dict:
            try:
                signature = str(row[0])
                name = str(cert.subject.native['common_name'])
                modulus = str(cert.public_key.native['public_key']['modulus'])
            except:
                continue
            if (cnt == 34071):
                cnt = cnt + 1
                continue
            fo.write(modulus)
            #fo.write(hex(int(modulus))[2:])
            fo.write("\n")
            print("Exc ", cnt)
            cnt = cnt + 1
            if (cnt > 400000):
                break
