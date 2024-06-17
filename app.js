const express = require('express');
const { exec } = require('child_process');
const multer=require('multer');
const fs = require('fs');
const expressLayouts = require('express-ejs-layouts');
const path=require('path');

const app = express();
app.use(express.static('stylesheet'));

app.set('view engine', 'ejs');
app.use(expressLayouts);
app.get('/home', (req, res) => {
    res.render('home', { title: 'home' });
    
});
app.get('/compress', (req, res) => {
    res.render('compress', { title: 'home' });
    
});
app.get('/decompress', (req, res) => {
    res.render('decompress', { title: 'home' });
    
});

const storage=multer.diskStorage({
    destination:function(req,file,cb){
        return cb(null,".");
    },
    filename:function(req,file,cb){
        return cb(null,`${file.originalname}`);
    }
})
const upload=multer({storage});
app.post('/compress',upload.single("fileinput"), (req, res) => {
    const exePath = path.join(__dirname, 'example.exe'); 
    const command = `"${exePath}" compress "${req.file.path}"`;

   
    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error executing command: ${error}`);
            return;
        }
            console.log(`Output: ${stdout}`);

            console.error(`Errors: ${stderr}`);
    });
    const filenameWithExtension = path.basename(req.file.path);
    setTimeout(function() {
        res.download(`${path.parse(filenameWithExtension).name}.bin`, (err) => {
            if (err) {
                console.error('Error downloading file:', err);
            } else {
                console.log('File downloaded successfully');
                fs.unlink(req.file.path, (err) => {
                    if (err) {
                        console.error('Error deleting original file:', err);
                    } else {
                        console.log('Original file deleted successfully');
                    }
                });
                fs.unlink(`${path.parse(filenameWithExtension).name}.bin`, (err) => {
                    if (err) {
                        console.error('Error deleting downloaded file:', err);
                    } else {
                        console.log('Downloaded file deleted successfully');
                    }
                });
            }
        });
    }, 3000);
});

app.post('/decompress',upload.single("fileinput"), (req, res) => {
    const exePath = path.join(__dirname, 'example.exe'); 
    const command = `"${exePath}" decompress "${req.file.path}"`;

    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error executing command: ${error}`);
            return;
        }
            console.log(`Output: ${stdout}`);

            console.error(`Errors: ${stderr}`);
    });
    const filenameWithExtension = path.basename(req.file.path);
    setTimeout(function() {
        res.download(`${path.parse(filenameWithExtension).name}.txt`, (err) => {
            if (err) {
                console.error('Error downloading file:', err);
            } else {
                console.log('File downloaded successfully');
                fs.unlink(req.file.path, (err) => {
                    if (err) {
                        console.error('Error deleting original file:', err);
                    } else {
                        console.log('Original file deleted successfully');
                    }
                });
                fs.unlink(`${path.parse(filenameWithExtension).name}.txt`, (err) => {
                    if (err) {
                        console.error('Error deleting downloaded file:', err);
                    } else {
                        console.log('Downloaded file deleted successfully');
                    }
                });
            }
        });
    }, 3000);
});

const PORT = process.env.PORT || 8090;
app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});
