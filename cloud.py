from googleapiclient.discovery import build
from googleapiclient.http import MediaFileUpload
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
import pickle
import os.path

# If modifying these SCOPES, delete the file token.pickle.
SCOPES = ["https://www.googleapis.com/auth/drive.file"]


def main():
    creds = None
    if os.path.exists("token.pickle"):
        with open("token.pickle", "rb") as token:
            creds = pickle.load(token)
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(
                r"C:\Users\pkhoa\OneDrive - VNU-HCMUS\Documents\GitHub\H-th-ng-b-o-m-t-th-ng-tin\client_secret_954634621810-u0ci6amt0t9oojpogpniq4k4h13nkbph.apps.googleusercontent.com.json",
                SCOPES,
            )
            creds = flow.run_local_server(port=0)
        with open("token.pickle", "wb") as token:
            pickle.dump(creds, token)

    service = build("drive", "v3", credentials=creds)

    # List of files to upload
    files_to_upload = [
        # replace with your file paths on your local machine below
        # Add more file paths as needed
    ]

    for file_path in files_to_upload:
        file_metadata = {
            "name": os.path.basename(file_path),
            "parents": [
                "1YljB2p4Cofeo1H3rx0Awyce4R6ijRZtw"
            ],  # replace with your folder ID
        }

        media = MediaFileUpload(
            file_path,
            mimetype="text/plain",
        )
        file = (
            service.files()
            .create(body=file_metadata, media_body=media, fields="id")
            .execute()
        )
        print(f"File ID: {file.get('id')} for file {file_path}")


if __name__ == "__main__":
    main()
