const char uploadPage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <title>IOT47 OTA</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 20px;
            text-align: center;
        }
        
        h1 {
            color: #333;
        }
        
        .upload-form {
            margin-top: 40px;
        }
        
        .upload-form input[type="file"] {
            display: none;
        }
        
        .upload-form label {
            display: inline-block;
            background-color: #3498db;
            color: #fff;
            padding: 10px 20px;
            border-radius: 4px;
            cursor: pointer;
            transition: background-color 0.3s;
        }
        
        .upload-form label:hover {
            background-color: #2980b9;
        }
        
        .upload-form input[type="submit"] {
            display: none;
        }
        
        .custom-button {
            display: inline-block;
            background-color: #2ecc71;
            color: #fff;
            padding: 10px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            transition: background-color 0.3s;
        }
        
        .custom-button:hover {
            background-color: #27ae60;
        }
        
        .progress-bar {
            width: 300px;
            height: 20px;
            background-color: #f1f1f1;
            border-radius: 4px;
            margin: 20px auto;
            overflow: hidden;
        }
        
        .progress-bar .progress {
            height: 100%;
            background-color: #3498db;
            width: 0;
            transition: width 0.3s;
        }
        
        #percent {
            margin-top: 10px;
            font-weight: bold;
        }
        
        #file-name {
            margin-top: 10px;
            font-weight: bold;
        }
    </style>
    <script>
      var isUploading = false;

        function uploadFile() {
          if (isUploading) {
                return;
            }
            var fileInput = document.getElementById('file');
            var progressBar = document.getElementById('progress');
            var percent = document.getElementById('percent');
            var fileName = document.getElementById('file-name');
            var file = fileInput.files[0];
            var xhr = new XMLHttpRequest();
            
            xhr.upload.onprogress = function(event) {
                if (event.lengthComputable) {
                    var progress = Math.round((event.loaded / event.total) * 100);
                    progressBar.style.width = progress + '%';
                    percent.innerHTML = progress + '%';
                }
            };
            
            xhr.onreadystatechange = function() {
                if (xhr.readyState === XMLHttpRequest.DONE) {
                    if (xhr.status === 200) {
                        percent.innerHTML = 'Tệp tin đã được tải lên thành công!';
                    } else {
                        percent.innerHTML = 'Đã xảy ra lỗi khi tải lên tệp tin.';
                    }
                    isUploading = false;
                }
            };
            
            xhr.open('POST', '/upload', true);
            var formData = new FormData();
            formData.append('file', file);
            xhr.send(formData);
            isUploading = true;
        }
        
        function displayFileName() {
            var fileInput = document.getElementById('file');
            var fileName = document.getElementById('file-name');
            fileName.innerHTML = fileInput.files[0].name;
        }
    </script>
</head>
<body>
    <h1>Cập nhật firmware</h1>
    <form class="upload-form">
        <label for="file" class="custom-button">Chọn tệp tin</label>
        <input type="file" name="file" id="file" required onchange="displayFileName()" accept=".bin"><br><br>
        <span id="file-name"></span>
        <input type="button" value="Tải lên" onclick="uploadFile()" class="custom-button">
    </form>
    <div class="progress-bar">
        <div class="progress" id="progress"></div>
    </div>
    <div id="percent"></div>
</body>
</html>

 )=====";