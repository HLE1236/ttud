# Web Học TTUD — Local C++ Learning & Judge Platform

Nền tảng học tập và luyện code C++ cho môn **Thuật toán ứng dụng (TTUD)** chạy trực tiếp trên máy cá nhân (localhost), không cần Internet sau khi tải về.

## Tính năng chính
- **Trình soạn thảo code tích hợp** (CodeMirror, theme Dracula, tự động đóng ngoặc, gợi ý cú pháp C++/STL, lưu nháp tự động).
- **Trình Debug trực tiếp trên web kiểu Code::Blocks** (đặt Breakpoint chấm đỏ trên số dòng, Step Over/Into/Out, Run to Cursor, Watches, Local variables, Call Stack) — tự động dùng **GDB** trên Windows/Linux và **LLDB** có sẵn của Xcode Command Line Tools trên **macOS** (hỗ trợ cả Apple Silicon và Intel).
- **Chấm bài tự động (Local Judge)** bằng `g++ -O2 -std=c++17` trên bộ testcase có sẵn trong `data/problems/`.
- **Lộ trình học & Lý thuyết** (`data/roadmap.json`, `data/theory.json`) kèm theo dõi tiến độ cá nhân.
- **Lưu lịch sử bài nộp & Ghi chú** cục bộ bằng SQLite (`data/ttud.db` tự động khởi tạo khi chạy lần đầu).

---

## 1. Yêu cầu hệ thống

1. **Python 3.8 trở lên**
   - Chỉ sử dụng thư viện chuẩn của Python, **không cần `pip install`** thêm thư viện ngoài.
2. **Trình biên dịch C++ (`g++`)** hỗ trợ `-std=c++17`
   - **Windows**: ưu tiên `C:\msys64\ucrt64\bin\g++.exe` (MSYS2 UCRT64), hoặc bất kỳ `g++` nào đã thêm vào `PATH` (MinGW-w64, TDM-GCC, Dev-C++/Code::Blocks MinGW...).
   - **macOS**:
     - Cài Xcode Command Line Tools (`xcode-select --install`) là có thể chạy ngay (server đã tích hợp sẵn header dự phòng `compat/bits/stdc++.h` cho Apple Clang).
     - Hoặc khuyên dùng Homebrew GCC để có `g++` chuẩn (hỗ trợ đầy đủ phần mở rộng GNU như `__gcd`, `<ext/pb_ds/...>`):
       ```bash
       brew install gcc
       ```
       `server.py` tự động tìm `g++-15` / `g++-14` / `g++-13` / `g++-12` trong `/opt/homebrew/bin` (Apple Silicon) và `/usr/local/bin` (Intel).
   - **Linux**: cài `g++` (`sudo apt install g++`) và có sẵn trong `PATH`.

Kiểm tra nhanh bằng Terminal / PowerShell:
```bash
python --version
g++ --version
```

Nếu máy Windows chưa có Python hoặc `g++`, có thể cài nhanh:
- **Python**: Tải từ [python.org](https://www.python.org/downloads/) (nhớ tick **Add Python to PATH** khi cài).
- **C++ (`g++`)**: Cài [MSYS2](https://www.msys2.org/), mở MSYS2 UCRT64 và chạy:
  ```bash
  pacman -S --needed mingw-w64-ucrt-x86_64-gcc
  ```

---

## 2. Tải về máy

> **Lưu ý nếu repository đang ở chế độ Private**: Chủ repo cần vào **Settings → Collaborators → Add people** để mời tài khoản GitHub của bạn trước thì bạn mới truy cập/tải được.

### Cách 1: Dùng Git (khuyên dùng)
```bash
git clone https://github.com/HLE1236/ttud.git
cd ttud
```

### Cách 2: Tải file ZIP (không cần cài Git)
1. Mở trang repository trên GitHub.
2. Bấm nút **Code** (màu xanh) → **Download ZIP**.
3. Giải nén file `.zip` vào một thư mục trên máy (tránh đường dẫn có quá nhiều ký tự đặc biệt nếu không cần thiết).

---

## 3. Cách chạy ứng dụng

### Trên Windows (Nhanh nhất)
- Mở thư mục vừa tải về và **nhấp đúp vào file `start.bat`**.
- Cửa sổ terminal sẽ khởi động server và tự động mở trình duyệt tại địa chỉ:
  `http://127.0.0.1:8765`
- Để tắt server, nhấn `Ctrl + C` trong cửa sổ terminal hoặc đóng cửa sổ đó lại.

### Trên macOS / Linux (Nhanh nhất)
1. Mở Terminal tại thư mục dự án và chạy:
   ```bash
   chmod +x start.sh start.command
   ./start.sh
   ```
   *(Trên macOS, bạn cũng có thể **nhấp đúp file `start.command`** trong Finder; nếu macOS báo chặn bảo mật lần đầu, bấm **Chuột phải → Open**).*

### Chạy bằng dòng lệnh (Windows / Linux / macOS)
```bash
python server.py 8765
```
*(Trên Linux/macOS có thể thay `python` bằng `python3`.)*

Sau đó mở trình duyệt và truy cập: **http://127.0.0.1:8765**

Nếu cổng `8765` đang bị ứng dụng khác sử dụng, bạn có thể đổi sang cổng khác (ví dụ `9000`):
```bash
python server.py 9000
```

---

## 4. Cấu trúc thư mục
```text
├── server.py          # Backend HTTP server (Python stdlib) + Local C++ Judge + SQLite
├── start.bat          # Script khởi chạy nhanh trên Windows
├── start.sh           # Script khởi chạy nhanh trên macOS / Linux
├── start.command      # File nhấp đúp mở nhanh trên macOS Finder
├── compat/            # Header dự phòng <bits/stdc++.h> cho Apple Clang trên macOS
├── static/            # Giao diện web (HTML, CSS, JS, CodeMirror)
└── data/
    ├── roadmap.json   # Dữ liệu lộ trình học
    ├── theory.json    # Dữ liệu lý thuyết
    └── problems/      # Đề bài, code mẫu (ref.cpp) và bộ testcases
```

---

## 5. Xử lý lỗi thường gặp
- **Lỗi không biên dịch / không chấm được bài**: Kiểm tra lại `g++ --version` trong Terminal. Nếu chưa có `g++` trong `PATH` (hoặc không nằm ở `C:\msys64\ucrt64\bin\g++.exe`), hãy cài MSYS2/MinGW và thêm thư mục `bin` chứa `g++.exe` vào `PATH`.
- **Lỗi `python` không được nhận diện**: Kiểm tra lại cài đặt Python hoặc thử lệnh `py server.py 8765`.
- **File `data/ttud.db`**: Được tạo tự động khi chạy lần đầu để lưu tiến độ của riêng máy bạn, không bị ghi đè khi cập nhật code mới.
