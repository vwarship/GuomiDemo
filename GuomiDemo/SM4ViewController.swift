//
//  SM4ViewController.swift
//  GuomiDemo
//
//  Created by 王军建 on 2017/10/28.
//  Copyright © 2017年 狗吃草. All rights reserved.
//

import UIKit

class SM4ViewController: UIViewController, UITextViewDelegate {
    @IBOutlet weak var keyTextView: UITextView!
    @IBOutlet weak var inTextView: UITextView!
    @IBOutlet weak var outTextView: UITextView!
    
    var key: String {
        set {
            keyTextView.text = newValue
        }
        get {
            return keyTextView.text ?? ""
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

    let keySize: Int = 16;

    override func viewDidLoad() {
        super.viewDidLoad()
        
        keyTextView.returnKeyType = .done
        inTextView.returnKeyType = .done
        outTextView.returnKeyType = .done
        
        keyTextView.delegate = self
        inTextView.delegate = self
        outTextView.delegate = self
        
        let randomNum = CUString(size: keySize)
        gm_generate_random(keySize, randomNum.toPtr())
        key = buffer2Hexstr(buffer: randomNum.toPtr(), buffer_len: keySize)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    
    @IBAction func touchEncrypt(_ sender: UIButton) {
        let keyBytes = hexstr2Buffer(hexstr: key)

        let inTextLength = inText.lengthOfBytes(using: .utf8)
        let encryptedTextLen = gm_sm4_calc_encrypted_data_memory_size(inTextLength)
        let encryptedText = CUString(size: encryptedTextLen + 1)
        gm_sm4_encrypt(keyBytes.buffer.toPtr(), inText, encryptedTextLen, encryptedText.toPtr())
        
        outText = buffer2Hexstr(buffer: encryptedText.toPtr(), buffer_len: encryptedTextLen)
        
        outTextView.selectAll(self)
    }
    
    @IBAction func touchDecrypt(_ sender: UIButton) {
        let keyBytes = hexstr2Buffer(hexstr: key)

        let encryptedData = hexstr2Buffer(hexstr: inText)
        let decryptedText = CUString(size: encryptedData.len + 1)
        gm_sm4_decrypt(keyBytes.buffer.toPtr(), encryptedData.buffer.toPtr(), encryptedData.len, decryptedText.toPtr())

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
