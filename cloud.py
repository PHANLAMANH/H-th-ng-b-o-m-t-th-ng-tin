from googleapiclient.discovery import build
from googleapiclient.http import MediaFileUpload
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
import pickle
import os.path

# If modifying these SCOPES, delete the file token.pickle.
SCOPES = ["https://www.googleapis.com/auth/drive.file"]


def main():
    """Shows basic usage of the Drive v3 API.
    Prints the names and ids of the first 10 files the user has access to.
    """
    creds = None
    # The file token.pickle stores the user's access and refresh tokens, and is
    # created automatically when the authorization flow completes for the first
    # time.
    if os.path.exists("token.pickle"):
        with open("token.pickle", "rb") as token:
            creds = pickle.load(token)
    # If there are no (valid) credentials available, let the user log in.
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(
                r"C:\Users\pkhoa\OneDrive - VNU-HCMUS\Documents\GitHub\H-th-ng-b-o-m-t-th-ng-tin\client_secret_954634621810-u0ci6amt0t9oojpogpniq4k4h13nkbph.apps.googleusercontent.com.json",
                SCOPES,
            )
            creds = flow.run_local_server(port=0)
        # Save the credentials for the next run
        with open("token.pickle", "wb") as token:
            pickle.dump(creds, token)

    service = build("drive", "v3", credentials=creds)

    # Call the Drive v3 API
    file_metadata = {
        "name": "keys.txt",
        "parents": ["1YljB2p4Cofeo1H3rx0Awyce4R6ijRZtw"],  # replace with your folder ID
    }

    media = MediaFileUpload(
        r"C:\Users\pkhoa\OneDrive - VNU-HCMUS\Documents\GitHub\H-th-ng-b-o-m-t-th-ng-tin\decrypted_message.txt",
        mimetype="text/plain",
    )
    file = (
        service.files()
        .create(body=file_metadata, media_body=media, fields="id")
        .execute()
    )
    print("File ID: {}".format(file.get("id")))


if __name__ == "__main__":
    main()
