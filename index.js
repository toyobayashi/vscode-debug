const addon = require('./build/Debug/md5.node')

const { join } = require('path')

function fileMD5(filepath) {
  const hash = require('crypto').createHash('md5')
  hash.setEncoding('hex')
  require('fs').createReadStream(filepath).pipe(hash).pipe(process.stdout)
}

function fileMD5Native(filepath) {
  return new Promise((resolve, reject) => {
    addon.calcFile(filepath, function (err, value) {
      if (err) {
        reject(err)
        return
      }
      resolve(value)
    })
  })
}

const filepath = join(__dirname, './video/bili22.mp4')
fileMD5Native(filepath).then(res => console.log(res)).catch(err => console.error(err))
// fileMD5(filepath)

console.log(1)
console.log(1)
console.log(1)
console.log(1)
