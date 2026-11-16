# Schema nội dung giảng dạy (lessons + guides)

Tất cả text dạng "prose" hỗ trợ markdown-lite: `code`, **đậm**, *nghiêng*, dòng bắt đầu bằng "- " là gạch đầu dòng, xuống dòng `\n` tách đoạn.
Giọng văn: tiếng Việt thân thiện, như anh/chị khoá trên giảng cho người **biết C++ cơ bản nhưng chưa vững thuật toán**. Ưu tiên "bắt đầu từ đâu, code thế nào", tránh hàn lâm.

## 1. `data/lessons/<chapterId>.json` — bài giảng theo chương

```json
{
  "chapter": "chap3",
  "intro": "prose: chương này giải quyết loại bài gì, vì sao cần học",
  "learning_path": {
    "guided":   ["pid", "pid"],
    "practice": ["pid", "..."],
    "exam":     ["pid", "..."]
  },
  "patterns": [
    {
      "id": "subset-gen",
      "name": "Dạng 1: Sinh tập con / dãy nhị phân",
      "when": ["dấu hiệu nhận biết khi đọc đề (prose ngắn)", "..."],
      "idea": "prose: ý tưởng cốt lõi dễ hiểu",
      "steps": ["Bước 1 viết code: ...", "Bước 2: ..."],
      "template_cpp": "code C++17 hoàn chỉnh, biên dịch được, chú thích tiếng Việt KHÔNG DẤU hoặc có dấu trên GẦN NHƯ MỌI DÒNG quan trọng",
      "trace": {
        "title": "Chạy tay với ví dụ ...",
        "input": "n = 3, a = [2, 3, 5]",
        "headers": ["Bước", "i", "x[]", "Tổng", "Hành động"],
        "rows": [["1", "1", "[1]", "2", "Gọi Try(2)"]],
        "note": "prose: nhận xét rút ra từ bảng"
      },
      "pitfalls": ["lỗi hay gặp ...", "..."],
      "problems": ["pid dễ", "pid vừa", "pid khó"]
    }
  ]
}
```

Quy tắc:
- `learning_path.guided`: 2–3 bài mẫu dễ nhất, mỗi bài BẮT BUỘC có `guide.json` thật kỹ.
- `practice`: sắp từ dễ đến khó. `exam`: bài có `exam_tags` (đề thi thật).
- Mỗi pid phải tồn tại trong `data/problems/`. Mỗi bài của chương nên xuất hiện ở ít nhất 1 pattern.
- 3–6 pattern mỗi chương. `trace.rows` 5–12 dòng.

## 2. `data/problems/<pid>/guide.json` — gợi ý mở dần từng tầng

```json
{
  "pattern": "subset-gen",
  "recognize": "prose: đọc đề thấy gì -> nghĩ ngay tới dạng nào, vì sao (dựa vào giới hạn n)",
  "idea": "prose: ý tưởng giải, trạng thái/biến cần lưu",
  "pseudocode": "mã giả nhiều dòng, plain text (hiển thị trong <pre>)",
  "skeleton_cpp": "code C++ biên dịch được: đã có #include, fast IO, ĐỌC INPUT ĐẦY ĐỦ, khai báo biến/hàm cần thiết, và các chỗ `// TODO: ...` mô tả cụ thể việc cần viết. KHÔNG chứa lời giải."
}
```

Lời giải đầy đủ là `ref.cpp` sẵn có (không sửa ref.cpp, meta.json).
