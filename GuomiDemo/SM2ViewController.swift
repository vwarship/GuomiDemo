//
//  SM2ViewController.swift
//  GuomiDemo
//
//  Created by 王军建 on 2017/10/27.
//  Copyright © 2017年 狗吃草. All rights reserved.
//

import UIKit

class SM2ViewController: UIViewController, UITextViewDelegate {
    @IBOutlet weak var randomNumberTextView: UITextView!
    @IBOutlet weak var publicKeyTextView: UITextView!
    @IBOutlet weak var privateKeyTextView: UITextView!
    @IBOutlet weak var generateKeyButton: UIButton!
    @IBOutlet weak var inTextView: UITextView!
    @IBOutlet weak var outTextView: UITextView!

    var randomNumber: String {
        set {
            randomNumberTextView.text = newValue
        }
        get {
            return randomNumberTextView.text ?? ""
        }
    }

    var publicKeyText: String {
        set {
            publicKeyTextView.text = newValue
        }
        get {
            return publicKeyTextView.text ?? ""
        }
    }
    
    var privateKeyText: String {
        set {
            privateKeyTextView.text = newValue
        }
        get {
            return privateKeyTextView.text ?? ""
        }
    }

    var inText: String {
        set {
            inTextView.text = newValue
        }
        get {
            return inTextView.text ?? ""
        }
    }
    
    var outText: String {
        set {
            outTextView.text = newValue
        }
        get {
            return outTextView.text ?? ""
        }
    }
    
    let bufferSize = 1024

    override func viewDidLoad() {
        super.viewDidLoad()

        inTextView.returnKeyType = .done
        inTextView.delegate = self
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    
    @IBAction func touchGenerateKeys(_ sender: UIButton) {
        let random = CString(size: bufferSize)
        gm_generate_random(random.toPtr())
        randomNumber = random.toString()
        
        let publicKey = CString(size: bufferSize)
        let privateKey = CString(size: bufferSize)
        gm_sm2_generate_keys(randomNumber, publicKey.toPtr(), privateKey.toPtr())
        publicKeyText = publicKey.toString()
        privateKeyText = privateKey.toString()
    }
    
    @IBAction func touchEncrypt(_ sender: UIButton) {
        let encryptedText = CUString(size: bufferSize)
        gm_sm2_encrypt(publicKeyText, inText, inText.count, encryptedText.toPtr())
        outText = buffer2Hexstr(buffer: encryptedText.toPtr(), buffer_len: encryptedText.strSize)
        
        outTextView.selectAll(self)

        let decryptedText = CUString(size: bufferSize)
        gm_sm2_decrypt(privateKeyText, encryptedText.toPtr(), encryptedText.strSize, decryptedText.toPtr())
    }
    
    @IBAction func touchDecrypt(_ sender: UIButton) {
        let encryptedText = hexstr2Buffer(hexstr: inText)
        let decryptedText = CUString(size: bufferSize)
        gm_sm2_decrypt(privateKeyText, encryptedText.buffer.toPtr(), encryptedText.len, decryptedText.toPtr())
        outText = decryptedText.toString()
    }
    
    func textView(_ textView: UITextView, shouldChangeTextIn range: NSRange, replacementText text: String) -> Bool {
        if text == "\n" {
            textView.resignFirstResponder()
            return true
        }
        
        return true
    }

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

}
