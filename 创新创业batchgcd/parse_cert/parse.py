import csv
import base64
from asn1crypto.x509 import Certificate
import mysql.connector

# Open data set
with open('./2018-10-23-1540260352-https_get_443_certs') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')

    # Connect to db
    print("Login to database:")
    db = mysql.connector.connect(
        host=input("host name: "),
        user=input("user name: "),
        passwd=input("password: "),
        database=input("database: ")
    )
    cursor = db.cursor()
    cnt = 1

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

            sql = "insert into name_signature_modulus(name, signature, modulus) values (%s,%s,%s)"
            value = (name, signature, modulus)
            cursor.execute(sql, value)
            print("Execute", cnt)
            cnt = cnt + 1
    db.commit()
