const  inputfile=document.querySelector('.input');
const getfile=document.querySelector('.btn');
const download=document.querySelector('.btn1');

getfile.addEventListener('click',function(){
    inputfile.click();
})

inputfile.addEventListener('change',function(){
    if (this.files.length === 0) {
        console.error('No file selected.');
        return;
    }
    download.classList.add('active');
})

